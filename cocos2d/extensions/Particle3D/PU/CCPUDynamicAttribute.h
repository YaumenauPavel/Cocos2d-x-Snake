/****************************************************************************
 Copyright (C) 2013 Henry van Merode. All rights reserved.
 Copyright (c) 2015-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __CC_PU_PARTICLE_3D_DYNAMIC_ATTRIBUTE_H__
#define __CC_PU_PARTICLE_3D_DYNAMIC_ATTRIBUTE_H__

#include "base/CCRef.h"
#include "math/CCMath.h"
#include "CCPUSimpleSpline.h"
#include <vector>

NS_CC_BEGIN

enum PUInterpolationType
{
    IT_LINEAR,
    IT_SPLINE
};

/** Comparer used for sorting vector in ascending order
*/
struct PUControlPointSorter
{
    bool operator()(const Vec2 &a, const Vec2 &b)
    {
        return a.x < b.x;
    }
};

/*	The DynamicAttribute class or its child classes encapsulate an attribute with specific (dynamic) behaviour.
@remarks
    This class provides a uniform interface for retrieving the value of an attribute, while the calculation of
    this value may vary. Each subclass provides its own implementation of the getValue() function and the calling
    application doesn't need to know about the underlying logic. A subclass could just return a value that has 
    previously been set, but it can also return a value that is randomly generated by a pre-defined min/max interval.
    The DynamicAttribute class is used in situations where different behaviour of a certain attribute is needed,
    but where implementation of this behaviour may not be scattered or duplicated within the application that needs 
    it.
*/
class CC_DLL PUDynamicAttribute : public Ref
{
public:
    enum DynamicAttributeType
    {
        DAT_FIXED,
        DAT_RANDOM,
        DAT_CURVED,
        DAT_OSCILLATE
    };

    /** Constructor
    */
    PUDynamicAttribute();

    /** Destructor
    */
    virtual ~PUDynamicAttribute();

    /** Virtual function that needs to be implemented by its childs.
    */
    virtual float getValue(float x = 0) = 0;

    /** Todo
    */
    DynamicAttributeType getType() const;

    /** Todo
    */
    void setType(DynamicAttributeType type);

    /** Returns true if one of the attributes was changed by an external source.
    */
    bool isValueChangedExternally() const;

    virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) = 0;
    virtual PUDynamicAttribute* clone() = 0;

protected:
    DynamicAttributeType _type;
    bool _valueChangedExternally;
};

/*	This class is an implementation of the DynamicAttribute class in its most simple form. It just returns a value
    that has previously been set.
@remarks
    Although use of a regular attribute within the class that needs it is preferred, its benefit is that it makes
    use of the generic 'getValue' mechanism of a DynamicAttribute.
*/
class CC_DLL PUDynamicAttributeFixed : public PUDynamicAttribute
{
public:
    /** Constructor
    */
    PUDynamicAttributeFixed();

    /** Copy constructor
    */
    PUDynamicAttributeFixed(const PUDynamicAttributeFixed &dynamicAttributeFixed);

    /** Destructor
    */
    ~PUDynamicAttributeFixed();

    /** Todo
    */
    virtual float getValue(float x = 0) override;

    /** Todo
    */
    virtual void setValue(float value);

    virtual PUDynamicAttributeFixed* clone() override;
    virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) override;

protected:
    float _value;
};

/* This class generates random values within a given minimum and maximum interval.
*/
class CC_DLL PUDynamicAttributeRandom : public PUDynamicAttribute
{
public:
    /** Constructor
    */
    PUDynamicAttributeRandom();

    /** Copy constructor
    */
    PUDynamicAttributeRandom(const PUDynamicAttributeRandom &dynamicAttributeRandom);

    /** Destructor
    */
    ~PUDynamicAttributeRandom();

    /** Todo
    */
    virtual float getValue(float x = 0) override;

    /** Todo
    */
    void setMin(float min);
    float getMin() const;
    void setMax(float max);
    float getMax() const;
    void setMinMax(float min, float max);

    virtual PUDynamicAttributeRandom* clone() override;
    virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) override;

protected:
    float _min, _max;
};

/* This is a more complex usage of the DynamicAttribute principle. This class returns a value on an curve.
@remarks
    After setting a number of control points, this class is able to interpolate a point on the curve that is based
    on these control points. Interpolation is done in different flavours. linear?provides linear interpolation
    of a value on the curve, while spline?generates a smooth curve and the returns a value that lies on that curve.
*/
class CC_DLL PUDynamicAttributeCurved : public PUDynamicAttribute
{
public:
    typedef std::vector<Vec2> ControlPointList;

    /** Constructor
    */
    PUDynamicAttributeCurved();
    PUDynamicAttributeCurved(PUInterpolationType interpolationType);

    /** Copy constructor
    */
    PUDynamicAttributeCurved(const PUDynamicAttributeCurved &dynamicAttributeCurved);

    /** Destructor
    */
    ~PUDynamicAttributeCurved();

    /** Get and set the curve type
    */
    void setInterpolationType(PUInterpolationType interpolationType);
    PUInterpolationType getInterpolationType() const;

    /** Todo
    */
    virtual float getValue(float x = 0) override;

    /** Todo
    */
    virtual void addControlPoint(float x, float y);

    /** Todo
    */
    const ControlPointList &getControlPoints() const;

    /** Todo
    */
    void processControlPoints();

    /** Todo
    */
    size_t getNumControlPoints() const;

    /** Todo
    */
    void removeAllControlPoints();

    virtual PUDynamicAttributeCurved* clone() override;
    virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) override;

protected:

    /** Todo
    */
    float _range;

    /** Todo
    */
    PUSimpleSpline _spline;

    /** Todo
    */
    PUInterpolationType _interpolationType;

    /** Todo
    */
    ControlPointList _controlPoints;

    /** Find an iterator that forms the low (left) value of the interval where x lies in.
    */
    inline ControlPointList::iterator findNearestControlPointIterator(float x);

    /** Helper functions
    */
    inline ControlPointList::iterator getFirstValidIterator();
    inline ControlPointList::iterator getLastValidIterator();
};

/* This class generates values based on an oscillating function (i.e. Sine).
*/
class CC_DLL PUDynamicAttributeOscillate : public PUDynamicAttribute
{
public:
    enum OscillationType
    {
        OSCT_SINE,
        OSCT_SQUARE
    };

    /** Constructor
    */
    PUDynamicAttributeOscillate();

    /** Copy constructor
    */
    PUDynamicAttributeOscillate(const PUDynamicAttributeOscillate &dynamicAttributeOscillate);

    /** Destructor
    */
    ~PUDynamicAttributeOscillate();

    /** Todo
    */
    virtual float getValue(float x = 0) override;

    /** Get and set the OscillationType
    */
    OscillationType getOscillationType() const;
    void setOscillationType(OscillationType oscillationType);

    /** Get and set the Frequency
    */
    float getFrequency() const;
    void setFrequency(float frequency);

    /** Get and set the Phase
    */
    float getPhase() const;
    void setPhase(float phase);

    /** Get and set the Base
    */
    float getBase() const;
    void setBase(float base);

    /** Get and set the Amplitude
    */
    float getAmplitude() const;
    void setAmplitude(float amplitude);

    virtual PUDynamicAttributeOscillate* clone() override;
    virtual void copyAttributesTo(PUDynamicAttribute* dynamicAttribute) override;

protected:
    OscillationType _oscillationType;
    float _frequency;
    float _phase;
    float _base;
    float _amplitude;
};

/* Helper class to do some generic calculation.
*/
class PUDynamicAttributeHelper
{
public:
    /* Return the value of a DynamicAttribute, given te x value.
    */
    float calculate(PUDynamicAttribute* dyn, float x, float defaultValue = 0.0f);
};
NS_CC_END

#endif
