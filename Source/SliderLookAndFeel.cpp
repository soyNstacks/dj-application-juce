/*
  ==============================================================================

    RotarySliderLookAndFeel.cpp
    Created: 11 Feb 2022 8:36:46pm
    Author:  taan0

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderLookAndFeel.h"

//==============================================================================
SpeedRotarySlider::SpeedRotarySlider()
{
    knobImage = ImageCache::getFromMemory(BinaryData::speedKnob_png, BinaryData::speedKnob_pngSize);

}

SpeedRotarySlider::~SpeedRotarySlider()
{

}

void SpeedRotarySlider::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    const double rotation = (slider.getValue()
        - slider.getMinimum())
        / (slider.getMaximum()
            - slider.getMinimum());
    const int frames = knobImage.getHeight() / knobImage.getWidth();
    const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
    const float radius = jmin(width / 2, height / 2) - 4.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    if (knobImage.isValid())
    {
        g.drawImage(knobImage,
            (int)rx,
            (int)ry,
            2 * (int)radius,
            2 * (int)radius,
            0,
            frameId * knobImage.getWidth(),
            knobImage.getWidth(),
            knobImage.getWidth());

        Path p;
        const float pointerLength = radius * 0.25f;
        const float pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer
        g.setColour(Colours::silver);
        g.fillPath(p);
    }
    else {
        //DBG("Knob not found"); 
    }

}

PositionRotarySlider::PositionRotarySlider()
{
    knobImage = ImageCache::getFromMemory(BinaryData::disk_png, BinaryData::disk_pngSize);
    
}

PositionRotarySlider::~PositionRotarySlider()
{

}

void PositionRotarySlider::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                                            float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    if (knobImage.isValid())
    {
        const double rotation = (slider.getValue()
            - slider.getMinimum())
            / (slider.getMaximum()
                - slider.getMinimum());
    
        auto imageArea = knobImage.getBounds().toFloat();
        imageArea.setBounds(x, y, width * 0.85f, height * 0.85f); 

        const float radius = jmin(imageArea.getWidth() / 2, imageArea.getHeight() / 2);
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        float fwidth = (float)width;
        float fheight = (float)height;
        const float angle = rotaryStartAngle
            + sliderPos
            * (rotaryEndAngle - rotaryStartAngle);

        auto transform = juce::AffineTransform::scale((rw) / knobImage.getWidth(), (rw) / knobImage.getHeight()).rotated(angle, rw / 2, rw / 2).translated(rx, ry);
        g.drawImageTransformed(knobImage, transform, false);
        
        g.setColour(juce::Colour::fromRGB(1, 185, 189));
        Path filledArc1;
        filledArc1.addPieSegment(rx, ry, rw + 1, rw + 1, rotaryStartAngle, angle, 0.95);
        g.fillPath(filledArc1);

        g.setColour(juce::Colour::fromRGBA(1, 185, 189, 150));
        Path midEllipse;
        midEllipse.addPieSegment(centreX - (radius * 0.4), centreY - (radius * 0.4), rw * 0.4, rw * 0.4, 0, juce::MathConstants<float>::twoPi, 0.10);
        g.fillPath(midEllipse);

        Path p;
        const float pointerLength = radius * 0.63f;
        const float pointerThickness = radius * 0.05f;
        p.addRectangle(-pointerThickness * 0.5f, -radius - 1, pointerThickness, pointerLength);

        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
        g.setColour(juce::Colour::fromRGB(200, 206, 205));
        g.fillPath(p);

        Rectangle<float> text_bounds(1.0f + width * 0.65f / 2.0f,
            centreY, width * 0.35f, centreY);
        g.setColour(juce::Colour::fromRGB(246, 246, 245));
        g.drawFittedText(String("DJ Application"), text_bounds.getSmallestIntegerContainer(),
            Justification::horizontallyCentred | Justification::centred, 1);
        
    }
    else
    {
        
    }
}

ReverbSlider::ReverbSlider()
{

}

ReverbSlider::~ReverbSlider()
{

}

void ReverbSlider::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    const float radius = jmin(width / 2, height / 2) - 4.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill outer
    const float outer_rx = centreX - (radius * 0.8);
    const float outer_ry = centreY - (radius * 0.8);
    g.setColour(juce::Colour(51, 49, 49));
    g.fillEllipse(outer_rx, outer_ry, rw * 0.8, rw * 0.8);

    // fill inner
    const float inner_rx = centreX - (radius * 0.65);
    const float inner_ry = centreY - (radius * 0.65);
    g.setColour(juce::Colour(18, 18, 18));
    g.fillEllipse(inner_rx, inner_ry, rw * 0.65, rw * 0.65);

    // fill innermost
    const float innermost_rx = centreX - (radius * 0.5);
    const float innermost_ry = centreY - (radius * 0.5);
    g.setColour(juce::Colour(30, 30, 30));
    g.fillEllipse(innermost_rx, innermost_ry, rw * 0.5, rw * 0.5);

    // outline
    g.setColour(juce::Colour(232, 232, 232));
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    g.setColour(juce::Colour::fromRGB(1, 185, 189));
    Path filledArc;
    filledArc.addPieSegment(rx, ry, rw + 1, rw + 1, rotaryStartAngle, angle, 0.85);

    g.fillPath(filledArc);

    g.setColour(juce::Colour::fromRGB(246, 246, 245));
    Path p;
    const float pointerLength = radius * 0.33f;
    const float pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

    g.fillPath(p);

    g.setColour(juce::Colours::beige);
    g.drawFittedText(slider.getName(), juce::Rectangle<int>(0, 0, width, height * 0.2), juce::Justification::centred, 2);
    

}

IIRFilterLevel::IIRFilterLevel()
{
    
}

IIRFilterLevel::~IIRFilterLevel()
{
    
}

void IIRFilterLevel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider)
{

    if (style == juce::Slider::LinearVertical)
    {
        auto sliderRect = juce::Rectangle<float>(x + (width * 0.3), y, width * 0.4, height);
        g.setColour(juce::Colour::fromRGB(246, 246, 245));
        g.drawRoundedRectangle(sliderRect, 4.0f, 1.0f);

        auto filledRect = juce::Rectangle<float>(x + (width * 0.3), sliderPos + 15.5f , width * 0.4, height - y - sliderPos);
        g.setColour(juce::Colour::fromRGB(1, 185, 189));
        g.fillRoundedRectangle(filledRect, 5.0f);

        drawLinearSliderThumb(g, x, y, width, height,
            sliderPos, minSliderPos, maxSliderPos,
            style, slider);
    }
    if (style == juce::Slider::LinearBarVertical)
    {
        auto sliderRect = juce::Rectangle<float>(x, sliderPos, width, height);
        g.setColour(juce::Colour::fromRGB(1, 185, 189));
        g.fillRoundedRectangle(sliderRect, 4.0f);
    }
}

void IIRFilterLevel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const Slider::SliderStyle style, Slider& slider) 
{   
    auto sliderRadius = (float)getSliderThumbRadius(slider);

    bool isDownOrDragging = slider.isEnabled() && (slider.isMouseOverOrDragging() || slider.isMouseButtonDown());

    if (style == juce::Slider::LinearVertical)
    {
        float kx, ky;

        if (style == Slider::LinearVertical)
        {
            kx = (float)x + (float)width * 0.5f;
            ky = sliderPos;

            g.setColour(juce::Colour::fromRGB(20, 20, 20));
            auto outer = juce::Rectangle<float>(kx - (sliderRadius * 1.25f), ky - 5.0f, sliderRadius * 2.5f, height * 0.05);
            g.fillRect(outer);
            g.setColour(juce::Colours::white);
            g.drawRect(outer);
            
            g.setColour(juce::Colour::fromRGB(50, 50, 50));
            auto inner = juce::Rectangle<float>(kx - (sliderRadius * 1.15f), ky - 5.0f + (height * 0.02), sliderRadius * 2.3f, height * 0.01);
            g.fillRect(inner);
            
            g.setColour(juce::Colour::fromRGB(40, 40, 40));
            g.drawRect(inner);

        }
        else
        {
            kx = sliderPos;
            ky = (float)y + (float)height * 0.5f;
            g.fillRect(Rectangle<float>(kx - 2.5f, ky - sliderRadius, 5.0f, sliderRadius * 2.0f));
        }
    }

}
