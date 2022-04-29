/*
  ==============================================================================

    RotarySliderLookAndFeel.h
    Created: 11 Feb 2022 8:36:46pm
    Author:  taan0

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class SpeedRotarySlider : public LookAndFeel_V4
{
public:
    SpeedRotarySlider();
    ~SpeedRotarySlider() override;

    /** customise look of slider */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
    
    /** image declaration */
    Image knobImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpeedRotarySlider)
};

//==============================================================================
class PositionRotarySlider : public LookAndFeel_V4
{

public:
    PositionRotarySlider();
    ~PositionRotarySlider() override;

    /** customise look of slider */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:

    /** image declaration */
    Image knobImage;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionRotarySlider)

};

//==============================================================================
class ReverbSlider : public LookAndFeel_V4
{
public:
    ReverbSlider();
    ~ReverbSlider() override;

    /** customise look of slider */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;


private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbSlider)
};

//==============================================================================
class IIRFilterLevel : public LookAndFeel_V2
{
public:
    IIRFilterLevel();
    ~IIRFilterLevel() override;

    /** customise look of slider */
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height, 
        float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;
    /** customise look of slider thumb */
    void drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider) override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IIRFilterLevel)
};