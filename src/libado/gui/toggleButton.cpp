/*
 * toggleButton.cpp
 *
 *  Created on: 25 Feb 2017
 *      Author: linbob
 */

#include "toggleButton.hpp"

using namespace tgui;


void ToggleButton::setChecked(bool flag){
	if (flag && !checked){
		// Check this radio button
		checked = true;
	}else if(!flag && checked){
        checked = false;
    }
}

bool ToggleButton::isChecked(){
	return checked;
}

void ToggleButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw the background of the button
	getRenderer()->draw(target, states);

	// If the button has a text then also draw the text
	target.draw(m_text, states);
}

void ToggleButtonRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	// Check if there is a background texture
	if (m_textureNormal.isLoaded())
	{
		if (((ToggleButton*)m_button)->m_mouseHover)
		{
			if (((ToggleButton*)m_button)->m_mouseDown && m_textureDown.isLoaded())
				target.draw(m_textureDown, states);
			else if (((ToggleButton*)m_button)->checked && m_textureDown.isLoaded())
				target.draw(m_textureDown, states);
			else if (m_textureHover.isLoaded())
				target.draw(m_textureHover, states);
			else
				target.draw(m_textureNormal, states);
		}
		else if (((ToggleButton*)m_button)->checked && m_textureDown.isLoaded())
			target.draw(m_textureDown, states);
		else
			target.draw(m_textureNormal, states);

		// When the edit box is focused then draw an extra image
		if (((ToggleButton*)m_button)->m_focused && m_textureFocused.isLoaded())
			target.draw(m_textureFocused, states);
	}
	else // There is no background texture
	{
		sf::RectangleShape button(m_button->getSize());
		button.setPosition(m_button->getPosition());

		if (((ToggleButton*)m_button)->m_mouseHover)
		{
			if (((ToggleButton*)m_button)->m_mouseDown)
				button.setFillColor(calcColorOpacity(m_backgroundColorDown, m_button->getOpacity()));
			else if (((ToggleButton*)m_button)->checked)
				button.setFillColor(calcColorOpacity(m_backgroundColorDown, m_button->getOpacity()));
			else
				button.setFillColor(calcColorOpacity(m_backgroundColorHover, m_button->getOpacity()));
		}
		else if (((ToggleButton*)m_button)->checked)
			button.setFillColor(calcColorOpacity(m_backgroundColorDown, m_button->getOpacity()));
		else
			button.setFillColor(calcColorOpacity(m_backgroundColorNormal, m_button->getOpacity()));

		target.draw(button, states);
	}

	// Draw the borders around the button
	if (m_borders != Borders{0, 0, 0, 0})
	{
		sf::Vector2f position = m_button->getPosition();
		sf::Vector2f size = m_button->getSize();

		// Draw left border
		sf::RectangleShape border({m_borders.left, size.y + m_borders.top});
		border.setPosition(position.x - m_borders.left, position.y - m_borders.top);
		border.setFillColor(calcColorOpacity(m_borderColor, m_button->getOpacity()));
		target.draw(border, states);

		// Draw top border
		border.setSize({size.x + m_borders.right, m_borders.top});
		border.setPosition(position.x, position.y - m_borders.top);
		target.draw(border, states);

		// Draw right border
		border.setSize({m_borders.right, size.y + m_borders.bottom});
		border.setPosition(position.x + size.x, position.y);
		target.draw(border, states);

		// Draw bottom border
		border.setSize({size.x + m_borders.left, m_borders.bottom});
		border.setPosition(position.x - m_borders.left, position.y + size.y);
		target.draw(border, states);
	}
}

std::shared_ptr<WidgetRenderer> ToggleButtonRenderer::clone(Widget* widget)
{
	auto renderer = std::make_shared<ToggleButtonRenderer>(*this);
	renderer->m_button = static_cast<ToggleButton*>(widget);
	return renderer;
}
