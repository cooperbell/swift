/*
 * Copyright (c) 2011 Tobias Markmann
 * Licensed under the simplified BSD license.
 * See Documentation/Licenses/BSD-simplified.txt for more information.
 */

/*
 * Copyright (c) 2014-2016 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#include <Swiften/Client/XMLBeautifier.h>

#include <sstream>
#include <stack>

#include <Swiften/Base/Log.h>
#include <Swiften/Parser/PlatformXMLParserFactory.h>

namespace Swift {

XMLBeautifier::XMLBeautifier(bool indention, bool coloring) : doIndention(indention), doColoring(coloring), intLevel(0), parser(nullptr), lastWasStepDown(false) {
    factory = new PlatformXMLParserFactory();
}

XMLBeautifier::~XMLBeautifier() {
    delete factory;
}

std::string XMLBeautifier::beautify(const std::string &text) {
    parser = factory->createXMLParser(this);
    intLevel = 0;
    buffer.str(std::string());
    parser->parse(text);
    delete parser;
    return buffer.str();
}

void XMLBeautifier::indent() {
    for (int i = 0; i < intLevel; ++i) {
        buffer << " ";
    }
}

// all bold but reset
// static const char colorBlue[] = "\x1b[01;34m";
static const char colorCyan[] = "\x1b[01;36m";
static const char colorGreen[] = "\x1b[01;32m";
// static const char colorMagenta[] = "\x1b[01;35m";
static const char colorRed[] = "\x1b[01;31m";
static const char colorReset[] = "\x1b[0m";
static const char colorYellow[] = "\x1b[01;33m";



std::string XMLBeautifier::styleTag(const std::string& text) const {
    std::string result;
    result += colorYellow;
    result += text;
    result += colorReset;
    return result;
}

std::string XMLBeautifier::styleNamespace(const std::string& text) const {
    std::string result;
    result += colorRed;
    result += text;
    result += colorReset;
    return result;
}

std::string XMLBeautifier::styleAttribute(const std::string& text) const {
    std::string result;
    result += colorGreen;
    result += text;
    result += colorReset;
    return result;
}
std::string XMLBeautifier::styleValue(const std::string& text) const {
    std::string result;
    result += colorCyan;
    result += text;
    result += colorReset;
    return result;
}

void XMLBeautifier::handleStartElement(const std::string& element, const std::string& ns, const AttributeMap& attributes) {
    if (doIndention) {
        if (intLevel) buffer << std::endl;
    }
    indent();
    buffer << "<" << (doColoring ? styleTag(element) : element);
    if (!ns.empty() && (!parentNSs.empty() && parentNSs.top() != ns)) {
        buffer << " ";
        buffer << (doColoring ? styleAttribute("xmlns") : "xmlns");
        buffer << "=";
        buffer << "\"" << (doColoring ? styleNamespace(ns) : ns) << "\"";
    }
    if (!attributes.getEntries().empty()) {
        for (const auto& entry : attributes.getEntries()) {
            buffer << " ";
            buffer << (doColoring ? styleAttribute(entry.getAttribute().getName()) : entry.getAttribute().getName());
            buffer << "=";
            buffer << "\"" << (doColoring ? styleValue(entry.getValue()) : entry.getValue()) << "\"";
        }
    }
    buffer << ">";
    ++intLevel;
    lastWasStepDown = false;
    parentNSs.push(ns);
}

void XMLBeautifier::handleEndElement(const std::string& element, const std::string& /* ns */) {
    --intLevel;
    parentNSs.pop();
    if (/*hadCDATA.top() ||*/ lastWasStepDown) {
        if (doIndention) {
            buffer << std::endl;
        }
        indent();
    }
    buffer << "</" << (doColoring ? styleTag(element) : element) << ">";
    lastWasStepDown = true;
}

void XMLBeautifier::handleCharacterData(const std::string& data) {
    buffer << data;
    lastWasStepDown = false;
}

}
