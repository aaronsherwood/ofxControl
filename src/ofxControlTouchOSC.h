#pragma once

#include "ofMain.h"
#include "Base64.h"


enum ofxControlTouchOscColor
{
    RED, GREEN, BLUE,
    YELLOW, PURPLE, GRAY,
    ORANGE, BROWN, PINK
};


class ofxControlTouchOscWidget
{
public:
    ofxControlTouchOscWidget(string name, float x, float y, float w, float h, ofxControlTouchOscColor color)
    {
        this->name = name;
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        setColor(color);
        scaleX = 1.0;
        scaleY = 1.0;
        oscManual = false;
    }
    
    virtual void setOscAddress(string oscAddress)
    {
        this->oscAddress = oscAddress;
        oscManual = true;
    }
    
    virtual void setColor(ofxControlTouchOscColor color)
    {
        if      (color == RED)      this->color = "red";
        else if (color == GREEN)    this->color = "green";
        else if (color == BLUE)     this->color = "blue";
        else if (color == YELLOW)   this->color = "yellow";
        else if (color == PURPLE)   this->color = "purple";
        else if (color == GRAY)     this->color = "gray";
        else if (color == ORANGE)   this->color = "orange";
        else if (color == BROWN)    this->color = "brown";
        else if (color == PINK)     this->color = "pink";
    }
    
    virtual void setScale(float scaleX, float scaleY)
    {
        this->scaleX = scaleX;
        this->scaleY = scaleY;
    }
    
    virtual string getXmlInner()
    {
        xml = "";
        string b64name = base64_encode(reinterpret_cast<const unsigned char*>(name.c_str()), name.length());
        xml += "<control name=\""+b64name+"\" ";
        xml += "type=\""+type+"\" ";
        xml += "x=\""+ofToString((int)(scaleX * x))+"\" y=\""+ofToString((int)(scaleY * y))+"\" ";
        xml += "w=\""+ofToString((int)(scaleX * w))+"\" h=\""+ofToString((int)(scaleY * h))+"\" color=\""+color+"\" ";
        getXml();
        xml += " >\n";
        xml += "</control>\n";
        return xml;
    }
    
    virtual void getXml()
    {
    };
    
    string xml;
    string type;
    string name;
    string oscAddress;
    string color;
    float x, y, w, h;
    float scaleX, scaleY;
    bool oscManual;
};


// labels, status displays

class ofxControlTouchOscMetaWidget : public ofxControlTouchOscWidget
{
public:
    ofxControlTouchOscMetaWidget(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, int textSize=14, bool background=false, bool outline=false) : ofxControlTouchOscWidget(name, x, y, w, h, color)
    {
        setTextSize(textSize);
        setBackground(background);
        setOutline(outline);
    }
    
    virtual void getXml()
    {
        xml += "size=\""+ofToString(textSize)+"\" background=\""+background+"\" outline=\""+outline+"\" ";
    }
    
    virtual void setTextSize(int textSize) {this->textSize = textSize;}
    virtual void setBackground(bool background) {this->background = background ? "true" : "false";}
    virtual void setOutline(bool outline) {this->outline = outline ? "true" : "false";}
    
    string background, outline;
    int textSize;
};

class ofxControlTouchOscLabel : public ofxControlTouchOscMetaWidget
{
public:
    ofxControlTouchOscLabel(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, int textSize=14, bool background=false, bool outline=false) : ofxControlTouchOscMetaWidget(name, x, y, w, h, color, textSize, background, outline)
    {
        label = name;
        type = (w >= h) ? "labelh" : "labelv";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscMetaWidget::getXml();
        string labelb64 = base64_encode(reinterpret_cast<const unsigned char*>(label.c_str()), label.length());
        xml += "text=\""+labelb64+"\" ";
    }
    
    virtual void setType(int t) {type = (t == 0) ? "labelh" : "labelv";}
    virtual void setLabel(string label) {this->label = label;}
    virtual void setHorizontal() {type = "labelh";}
    virtual void setVertical() {type = "labelv";}
    
    string label;
};

class ofxControlTouchOscBattery : public ofxControlTouchOscMetaWidget
{
public:
    ofxControlTouchOscBattery(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, int textSize=14, bool background=false, bool outline=false) : ofxControlTouchOscMetaWidget(name, x, y, w, h, color, textSize, background, outline)
    {
        type = (w >= h) ? "batteryh" : "batteryv";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscMetaWidget::getXml();
    }
    
    virtual void setType(int t) {type = (t == 0) ? "batteryh" : "batteryv";}
    virtual void setHorizontal() {type = "batteryh";}
    virtual void setVertical() {type = "batteryv";}
};

class ofxControlTouchOscTime : public ofxControlTouchOscMetaWidget
{
public:
    ofxControlTouchOscTime(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, int textSize=14, bool background=false, bool outline=false, bool show_seconds=false) : ofxControlTouchOscMetaWidget(name, x, y, w, h, color, textSize, background, outline)
    {
        setShowSeconds(show_seconds);
        type = (w >= h) ? "timeh" : "timev";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscMetaWidget::getXml();
        xml += "seconds=\""+show_seconds+"\" ";
    }
    
    virtual void setType(int t) {type = (t == 0) ? "timeh" : "timev";}
    virtual void setShowSeconds(bool show_seconds) {this->show_seconds = show_seconds ? "true" : "false";}
    virtual void setHorizontal() {type = "timeh";}
    virtual void setVertical() {type = "timev";}
    
    string show_seconds;
};


// widgets with numerical values

class ofxControlTouchOscNumericWidget : public ofxControlTouchOscWidget
{
public:
    ofxControlTouchOscNumericWidget(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0) : ofxControlTouchOscWidget(name, x, y, w, h, color)
    {
        this->min = min;
        this->max = max;
    }
    
    virtual void getXml()
    {
        xml += "scalef=\""+ofToString(min)+"\" scalet=\""+ofToString(max)+"\" ";
        if (oscManual) {
            string oscAddressb64 = base64_encode(reinterpret_cast<const unsigned char*>(oscAddress.c_str()), oscAddress.length());
            xml += "osc_cs=\""+oscAddressb64+"\" ";
        }
    }
    
    virtual void setRange(float min, float max) { this->min = min; this->max = max; }
    virtual void setMin(float min) { this->min = min; }
    virtual void setMax(float max) { this->max = max; }
    
    float min, max;
};

class ofxControlTouchOscFader : public ofxControlTouchOscNumericWidget
{
public:
    ofxControlTouchOscFader(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, bool inverted=false, bool centered=false, bool responseRelative=false) : ofxControlTouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setInverted(inverted);
        setCentered(centered);
        setResponseRelative(responseRelative);
        type = (w >= h) ? "faderh" : "faderv";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscNumericWidget::getXml();
        xml += "response=\""+response+"\" inverted=\""+inverted+"\" centered=\""+centered+"\" ";
    }
    
    virtual void setType(int t) {type = (t == 0) ? "faderh" : "faderv";}
    virtual void setHorizontal() {type="faderh";}
    virtual void setVertical() {type="faderv";}
    virtual void setInverted(bool inverted) { this->inverted = inverted ? "true" : "false"; }
    virtual void setCentered(bool centered) { this->centered = centered ? "true" : "false"; }
    virtual void setResponseRelative(bool responseRelative) { this->response = responseRelative ? "relative" : "absolute"; }
    
    string inverted, centered, response;
};

class ofxControlTouchOscRotary : public ofxControlTouchOscFader
{
public:
    ofxControlTouchOscRotary(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, bool inverted=false, bool centered=false, bool responseRelative=false, bool norollover=false) : ofxControlTouchOscFader(name, x, y, w, h, color, min, max, inverted, centered, responseRelative)
    {
        setNoRollover(norollover);
        type = (w >= h) ? "rotaryh" : "rotaryv";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscFader::getXml();
        xml += "norollover=\""+norollover+"\" ";
    }
    
    virtual void setHorizontal() {type="rotaryh";}
    virtual void setVertical() {type="rotaryv";}
    virtual void setNoRollover(bool norollover) {this->norollover = (norollover) ? "true" : "false"; }
    
    string norollover;
};

class ofxControlTouchOscLed : public ofxControlTouchOscNumericWidget
{
public:
    ofxControlTouchOscLed(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0) : ofxControlTouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        type = "led";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscNumericWidget::getXml();
    }
};

class ofxControlTouchOscButton : public ofxControlTouchOscNumericWidget
{
public:
    ofxControlTouchOscButton(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, bool local_off=false, bool velocity=false) : ofxControlTouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setLocalOff(local_off);
        setVelocity(velocity);
        type = "push";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscNumericWidget::getXml();
        xml += "local_off=\""+local_off+"\" velocity=\""+velocity+"\" ";
    }
    
    virtual void setLocalOff(bool local_off) {this->local_off = local_off ? "true" : "false";}
    virtual void setVelocity(bool velocity) {this->velocity = velocity ? "1" : "0";}
    
    string velocity, local_off;
};

class ofxControlTouchOscToggle : public ofxControlTouchOscNumericWidget
{
public:
    ofxControlTouchOscToggle(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, bool local_off=false) : ofxControlTouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setLocalOff(local_off);
        type = "toggle";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscNumericWidget::getXml();
        xml += "local_off=\""+local_off+"\" ";
    }
    
    virtual void setLocalOff(bool local_off) {this->local_off = local_off ? "true" : "false";}
    
    string local_off;
};

class ofxControlTouchOscEncoder : public ofxControlTouchOscNumericWidget
{
public:
    ofxControlTouchOscEncoder(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, bool local_off=false) : ofxControlTouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        type = "encoder";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscNumericWidget::getXml();
    }
};


class ofxControlTouchOscXy : public ofxControlTouchOscNumericWidget
{
public:
    ofxControlTouchOscXy(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, bool inverted_x=false, bool inverted_y=false) : ofxControlTouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setInvertedX(inverted_x);
        setInvertedY(inverted_y);
        type = "xy";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscNumericWidget::getXml();
        xml += "inverted_x=\""+ofToString(inverted_x)+"\" inverted_y=\""+ofToString(inverted_y)+"\" ";
    }
    
    virtual void setInvertedX(bool inverted_x) { this->inverted_x = inverted_x ? "true" : "false"; }
    virtual void setInvertedY(bool inverted_y) { this->inverted_y = inverted_y ? "true" : "false"; }
    
    string inverted_x, inverted_y;
};


class ofxControlTouchOscMultiPush : public ofxControlTouchOscNumericWidget
{
public:
    ofxControlTouchOscMultiPush(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, int number_x=2, int number_y=2, bool local_off=false) : ofxControlTouchOscNumericWidget(name, x, y, w, h, color, min, max) {
        setLocalOff(local_off);
        setNumberX(number_x);
        setNumberY(number_y);
        type = "multipush";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscNumericWidget::getXml();
        xml += "number_x=\""+ofToString(number_x)+"\" number_y=\""+ofToString(number_y)+"\" local_off=\""+local_off+"\" ";
    }
    
    virtual void setLocalOff(bool local_off) {this->local_off = local_off ? "true" : "false";}
    virtual void setNumberX(int number_x) {this->number_x = number_x;}
    virtual void setNumberY(int number_y) {this->number_y = number_y;}
    
    int number_x, number_y;
    string local_off;
};

class ofxControlTouchOscMultiToggle : public ofxControlTouchOscMultiPush
{
public:
    ofxControlTouchOscMultiToggle(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, int number_x=2, int number_y=2, bool local_off=false, bool ex_mode=false) : ofxControlTouchOscMultiPush(name, x, y, w, h, color, min, max)
    {
        setExclusive(ex_mode);
        type = "multitoggle";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscMultiPush::getXml();
        xml += "ex_mode=\""+ex_mode+"\" ";
    }
    
    virtual void setExclusive(bool ex_mode) {this->ex_mode = ex_mode ? "true" : "false";}
    
    string local_off, ex_mode;
};

class ofxControlTouchOscMultiFader : public ofxControlTouchOscNumericWidget
{
public:
    ofxControlTouchOscMultiFader(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, int number=2, bool inverted=false, bool centered=false) : ofxControlTouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setNumber(number);
        setInverted(inverted);
        setCentered(centered);
        type = (w >= h) ? "multifaderh" : "multifaderv";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscNumericWidget::getXml();
        xml += "number=\""+ofToString(number)+"\" inverted=\""+inverted+"\" centered=\""+centered+"\" ";
    }
    
    virtual void setType(int t) {type = (t == 0) ? "multifaderh" : "multifaderv";}
    virtual void setNumber(int number) {this->number = number;}
    virtual void setInverted(bool inverted) { this->inverted = inverted ? "true" : "false"; }
    virtual void setCentered(bool centered) { this->centered = centered ? "true" : "false"; }
    
    int number;
    string inverted, centered;
};

class ofxControlTouchOscMultiXy : public ofxControlTouchOscXy
{
public:
    ofxControlTouchOscMultiXy(string name, float x, float y, float w, float h, ofxControlTouchOscColor color=RED, float min=0.0, float max=1.0, bool inverted_x=false, bool inverted_y=false) : ofxControlTouchOscXy(name, x, y, w, h, color, min, max)
    {
        type = "multixy";
    }
    
    virtual void getXml()
    {
        ofxControlTouchOscXy::getXml();
    }
};



class ofxControlTouchOscPage
{
public:
    ~ofxControlTouchOscPage();
    ofxControlTouchOscPage(string name, ofxControlTouchOscColor color=YELLOW);
    void setScale(float scaleX, float scaleY);
    void setDefaultWidgetColor(ofxControlTouchOscColor color);
    string getXml();
    
    ofxControlTouchOscLabel* addLabel(string name, float x, float y, float w, float h);
    ofxControlTouchOscTime* addTime(string name, float x, float y, float w, float h);
    ofxControlTouchOscBattery* addBattery(string name, float x, float y, float w, float h);
    ofxControlTouchOscFader* addFader(string name, float x, float y, float w, float h);
    ofxControlTouchOscRotary* addRotary(string name, float x, float y, float w, float h);
    ofxControlTouchOscLed* addLed(string name, float x, float y, float w, float h);
    ofxControlTouchOscButton* addButton(string name, float x, float y, float w, float h);
    ofxControlTouchOscToggle* addToggle(string name, float x, float y, float w, float h);
    ofxControlTouchOscEncoder* addEncoder(string name, float x, float y, float w, float h);
    ofxControlTouchOscXy* addXy(string name, float x, float y, float w, float h);
    ofxControlTouchOscMultiPush* addMultiPush(string name, float x, float y, float w, float h);
    ofxControlTouchOscMultiToggle* addMultiToggle(string name, float x, float y, float w, float h);
    ofxControlTouchOscMultiFader* addMultiFader(string name, float x, float y, float w, float h);
    ofxControlTouchOscMultiXy* addMultiXy(string name, float x, float y, float w, float h);
    
    vector<ofxControlTouchOscWidget *> & getWidgets() {return widgets;}
    
private:
    
    vector<ofxControlTouchOscWidget *> widgets;
    string name;
    float scaleX, scaleY;
    ofxControlTouchOscColor defaultColor;
};


class ofxControlTouchOsc
{
public:
    ofxControlTouchOsc();
    ~ofxControlTouchOsc();

    ofxControlTouchOscPage* addPage(string name);
    void addPage(ofxControlTouchOscPage* newPage);
    void setDefaultColor(ofxControlTouchOscColor color);
    void setScale(float scaleX, float scaleY);
    void save(string name);
    string getXml();
    
private:
    vector<ofxControlTouchOscPage *> pages;
    float scaleX, scaleY;
    ofxControlTouchOscColor defaultColor;
    bool customResolution;
};

