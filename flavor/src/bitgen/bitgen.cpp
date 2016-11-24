/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */


// Includes
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include "DOMTreeErrorReporter.hpp"

#include <string.h>
#include <stdlib.h>

// Flavor library
#include <flavor.h>


static char* gXmlFile               = 0;     // The XML file
static bool  gDoNamespaces          = true;  // Namespace processing should be done?
static bool  gDoSchema              = true;  // Schema processing should be done?

// Particle unique attribution constraint checking and particle derivation restriction checking? 
static bool  gSchemaFullChecking    = true; 

static bool  gDoCreate              = false; // Entity reference nodes needs to be created?
static XercesDOMParser::ValSchemes gValScheme = XercesDOMParser::Val_Always; // Always validate

static char* gOutputFile            = 0;

// Parsable element information
static int p_aligned  = 0; // align
static int p_big      = 1; // 0: little-endian, 1: big-endian
static int p_bitLen   = 0; // parse size



void usage()
{
    cout << "\nUsage:\n"
            "    bitgen -ofile=<out file> -schema=[y|n] -full=[y|n] <XML file>\n\n"
            "This program parses an XML file (with validation), and builds the DOM tree.\n"
            "It then uses the Flavor run-time library (the Bitstream class) to write the\n"
            "corresponding binary data.\n"
            "Options:\n"
            "    -ofile   Specify the output file to write the bitstream.\n"
            "    -schema  Validate with schema.\n"
            "    -full    Perform full checking (includes PUA constraint and PDR checkings).\n"
            "    -?       Show this help.\n\n"
         <<  endl;
}


int main(int argC, char* argV[])
{
    int retval = 0;
    int schema = 0;

    // Initialize the XML4C2 system
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException &e) {
        cerr << "Error during initialization.\n  Msg: " << StrX(e.getMessage()) << endl;
        return 1;
    }

    // Check if we have enough data
    if (argC < 5) {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }

    int parmInd;
    for (parmInd = 1; parmInd < argC; parmInd++) {
        // Break out on first parm not starting with a dash
        if (argV[parmInd][0] != '-') 
            break;

        // Watch for special case help request
        if (!strcmp(argV[parmInd], "-?")) {
            usage();
            XMLPlatformUtils::Terminate();
            return 2;
        }
        else if (!strncmp(argV[parmInd], "-ofile=", 7)) 
            gOutputFile = &(argV[parmInd][7]);
        else if (!strncmp(argV[parmInd], "-schema=", 8)) { 
           if (argV[parmInd][8] == 'Y' || argV[parmInd][8] == 'y') {
                schema = 1;
                gValScheme = XercesDOMParser::Val_Always;
            }
            else {
                schema = 0;
                gValScheme = XercesDOMParser::Val_Auto;
            }
        }
        else if (!strncmp(argV[parmInd], "-full=", 6)) { 
           if (argV[parmInd][6] == 'Y' || argV[parmInd][6] == 'y') {
                gSchemaFullChecking = true;
            }
            else {
                gSchemaFullChecking = false;
            }
        }
        else 
            cerr << "Unknown option '" << argV[parmInd] << "', ignoring it.\n" << endl;
    }

    // And now we have to have only one parameter left and it must be the file name.
    if (parmInd + 1 != argC) {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }
    gXmlFile = argV[parmInd];

    // Create our parser, then attach an error handler to the parser.
    XercesDOMParser *parser = new XercesDOMParser;
    parser->setValidationScheme(gValScheme);
    parser->setDoNamespaces(gDoNamespaces);
    parser->setDoSchema(gDoSchema);
    parser->setValidationSchemaFullChecking(gSchemaFullChecking);
    parser->setCreateEntityReferenceNodes(gDoCreate);

    DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter();
    parser->setErrorHandler(errReporter);

    // Parse the XML file, catching any XML exceptions that might propogate out of it.
    bool errorsOccured = false;
    try {
        parser->parse(gXmlFile);
    }
    catch (const XMLException& e) {
        cerr << "An error occurred during parsing\n  Msg: " << StrX(e.getMessage()) << endl;
        errorsOccured = true;
    }
    catch (const DOMException& e) {
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];
        cerr << "\nDOM Error during parsing: '" << gXmlFile << "'\n  Code: " << e.code << endl;
        if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
            cerr << "Msg: " << StrX(errText) << endl;
        errorsOccured = true;
    }
    catch (...) {
        cerr << "An error occurred during parsing.\n" << endl;
        errorsOccured = true;
    }

    // If the parse was successful, output the document data from the DOM tree
    if (!errorsOccured && !errReporter->getSawErrors())  {
        try {
            XMLCh tempXMLStr[100];
            char tempStr[100];

            // Get the DOM representation
            DOMDocument *doc = parser->getDocument();
           
            // Get all element nodes
            XMLString::transcode("*", tempXMLStr, 99);
            DOMNodeList *nodes = doc->getElementsByTagName(tempXMLStr); 
            
            // Create an output bitstream
            Bitstream bs(gOutputFile, BS_OUTPUT);
            
            int nodeInd;
            int maxNodeInd = nodes->getLength();
            for (nodeInd = 0; nodeInd < maxNodeInd; nodeInd++) {
                // Get the nodeInd-th element node
                DOMNode *node = nodes->item(nodeInd);

                // Get the name of the element
                const XMLCh *name = ((DOMElement *)node)->getTagName();
                XMLString::transcode(name, tempStr, 99);

                // If the aligned attr is specified, then align the output bitstream
                XMLString::transcode("aligned", tempXMLStr, 99);
                if (((DOMElement *)node)->hasAttribute(tempXMLStr)) {
                    const XMLCh *aligned = ((DOMElement *)node)->getAttribute(tempXMLStr);
                    XMLString::transcode(aligned, tempStr, 99); 
                    p_aligned = atol(tempStr);
                    if (p_aligned)
                        bs.align(p_aligned);
                    p_aligned = 0;
                }

                // If the bitLen attr is specified, then it must be parsable
                XMLString::transcode("bitLen", tempXMLStr, 99);
                if (((DOMElement *)node)->hasAttribute(tempXMLStr)) {
                    const XMLCh *bitLen = ((DOMElement *)node)->getAttribute(tempXMLStr);
                    XMLString::transcode(bitLen, tempStr, 99); 
                    p_bitLen = atol(tempStr);

                    if (p_bitLen > 0) {

                        // Get byte-ordering method
                        XMLString::transcode("big", tempXMLStr, 99);
                        const XMLCh *big = ((DOMElement *)node)->getAttribute(tempXMLStr);
                        XMLString::transcode(big, tempStr, 99); 
                        if (XMLString::equals("false", tempStr))
                            p_big = 0;
                        else
                            p_big = 1;

                        // Get the value of the element
                        DOMNode *child = node->getFirstChild();
                        while (child->getNodeType() != DOMNode::TEXT_NODE)
                            child = child->getNextSibling();
                        const XMLCh *valueXML = child->getNodeValue();
                        char value[100];
                        XMLString::transcode(valueXML, value, 99);

                        // Get the type of the element
                        if (schema) {
                            const DOMTypeInfo *type = ((DOMElement *)node)->getTypeInfo();
                            const XMLCh *typeName = type->getName();
                            XMLString::transcode(typeName, tempStr, 99);
                        }
                        else {
                            XMLString::transcode("type", tempXMLStr, 99);
                            const XMLCh *type = ((DOMElement *)node)->getAttribute(tempXMLStr);
                            XMLString::transcode(type, tempStr, 99); 
                        }

                        if (XMLString::equals("flSChar", tempStr) || XMLString::equals("unsignedChar", tempStr))
                            bs.putbits(value[0], p_bitLen);
                        else if (XMLString::equals("flFloat", tempStr)) {
                            float v = (float)atof(value);
                            if (p_big)
                                bs.putfloat(v);
                            else
                                bs.little_putfloat(v);
                        }
                        else if (XMLString::equals("flDouble", tempStr)) {
                            double v = atof(value);
                            if (p_big)
                                bs.putdouble(v);
                            else
                                bs.little_putdouble(v);
                        }
                        // All others are considered type int
                        else {
                            int v = atol(value);
                            if (p_big)
                                bs.putbits(v, p_bitLen);
                            else
                                bs.little_putbits(v, p_bitLen);
                        }
                    }
                }
            }
            bs.flushbits();
        }
        catch (XMLException& e) {
            cerr << "An error occurred getting attributes.\n  Msg: " << StrX(e.getMessage()) << endl;
            retval = 4;
        }
    }
    else retval = 4;

    // Clean up the error handler
    delete errReporter;

    // Delete the parser itself; must be done prior to calling Terminate, below.
    delete parser;

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return retval;
}

