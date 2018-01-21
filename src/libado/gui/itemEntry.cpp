/*
 * lootEntry.cpp
 *
 *  Created on: 9 Oct 2016
 *      Author: linbob
 */

#include "itemEntry.hpp"

void ItemEntry::sliderChange(tgui::Label::Ptr valueLabel, tgui::EditBox::Ptr textBox, tgui::Slider::Ptr slider)
{
	textBox->setText(std::to_string(slider->getValue()));
}

void ItemEntry::init(anax::Entity& e, tgui::Theme::Ptr theme){
	item = e;

	tgui::Color fontColor(255, 255, 255);

	//sprite thumbnail
	SpriteComponent& sprite = e.getComponent<SpriteComponent>();
	SpriteWidget::Ptr thumbnail = std::make_shared<SpriteWidget>(sprite.sprite.get());
	thumbnail->setPosition(0, 0);
	add(thumbnail, "item_thumbnail");

	//item name
	NameComponent& n = e.getComponent<NameComponent>();
	tgui::Label::Ptr nameLabel = std::make_shared<tgui::Label>();
	nameLabel->setTextColor(fontColor);
	nameLabel->setTextSize(14);
	nameLabel->setSize(getSize().x * 0.6, 30);
	nameLabel->setText(n.name);
	nameLabel->setPosition(sprite.sprite->getGlobalBounds().width + 4, 0);
	add(nameLabel, "item_name_label");

	ValueComponent& v = e.getComponent<ValueComponent>();
	tgui::Label::Ptr valueLabel = std::make_shared<tgui::Label>();
	valueLabel->setTextColor(fontColor);
	valueLabel->setTextSize(14);
	valueLabel->setSize(getSize().x * 0.16, 30);
	valueLabel->setText(std::to_string(v.getValue()));
	valueLabel->setPosition(getSize().x - (valueLabel->getSize().x + 2), 0);
	add(valueLabel, "item_value_label");

	if(e.hasComponent<StackableComponent>()){
		StackableComponent& s = e.getComponent<StackableComponent>();
		tgui::Label::Ptr stackLabel = std::make_shared<tgui::Label>();
		stackLabel->setTextColor(fontColor);
		stackLabel->setTextSize(14);
		stackLabel->setSize(getSize().x * 0.16, 30);
		stackLabel->setText(std::to_string(s.count));
		stackLabel->setPosition(valueLabel->getPosition().x - (stackLabel->getSize().x + 12), 0);
		add(stackLabel, "item_stack_label");
	}
}

void ItemEntry::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	const sf::View& view = target.getView();

	// Calculate the scale factor of the view
	float scaleViewX = target.getSize().x / view.getSize().x;
	float scaleViewY = target.getSize().y / view.getSize().y;

	// Get the global position
	sf::Vector2f topLeftPosition = {((getAbsolutePosition().x - view.getCenter().x + (view.getSize().x / 2.f)) * view.getViewport().width) + (view.getSize().x * view.getViewport().left),
									((getAbsolutePosition().y - view.getCenter().y + (view.getSize().y / 2.f)) * view.getViewport().height) + (view.getSize().y * view.getViewport().top)};
	sf::Vector2f bottomRightPosition = {(getAbsolutePosition().x + getSize().x - view.getCenter().x + (view.getSize().x / 2.f)) * view.getViewport().width + (view.getSize().x * view.getViewport().left),
										(getAbsolutePosition().y + getSize().y - view.getCenter().y + (view.getSize().y / 2.f)) * view.getViewport().height + (view.getSize().y * view.getViewport().top)};

	// Get the old clipping area
	GLint scissor[4];
	glGetIntegerv(GL_SCISSOR_BOX, scissor);

	// Calculate the clipping area
	GLint scissorLeft = std::max(static_cast<GLint>(topLeftPosition.x * scaleViewX), scissor[0]);
	GLint scissorTop = std::max(static_cast<GLint>(topLeftPosition.y * scaleViewY), static_cast<GLint>(target.getSize().y) - scissor[1] - scissor[3]);
	GLint scissorRight = std::min(static_cast<GLint>(bottomRightPosition.x * scaleViewX), scissor[0] + scissor[2]);
	GLint scissorBottom = std::min(static_cast<GLint>(bottomRightPosition.y * scaleViewY), static_cast<GLint>(target.getSize().y) - scissor[1]);

	if (scissorRight < scissorLeft)
		scissorRight = scissorLeft;
	else if (scissorBottom < scissorTop)
		scissorTop = scissorBottom;

	// Set the clipping area
	glScissor(scissorLeft, target.getSize().y - scissorBottom, scissorRight - scissorLeft, scissorBottom - scissorTop);

	// Set the position
	states.transform.translate(getPosition());

	// Draw the background
	if (m_backgroundColor != sf::Color::Transparent)
	{
		sf::RectangleShape background(getSize());
		if(m_mouseHover){
			background.setFillColor(tgui::calcColorOpacity(getRenderer()->m_hoverBackgroundColor, getOpacity()));
		}else{
			background.setFillColor(tgui::calcColorOpacity(m_backgroundColor, getOpacity()));
		}
		target.draw(background, states);
	}

	// Draw the widgets
	drawWidgetContainer(&target, states);

	// Reset the old clipping area
	glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);

	// Draw the borders around the panel
	if (getRenderer()->m_borders != tgui::Borders{0, 0, 0, 0})
	{
		tgui::Borders& borders = getRenderer()->m_borders;
		sf::Vector2f size = getSize();

		// Draw left border
		sf::RectangleShape border({borders.left, size.y + borders.top});
		border.setPosition(-borders.left, -borders.top);
		border.setFillColor(tgui::calcColorOpacity(getRenderer()->m_borderColor, getOpacity()));
		target.draw(border, states);

		// Draw top border
		border.setSize({size.x + borders.right, borders.top});
		border.setPosition(0, -borders.top);
		target.draw(border, states);

		// Draw right border
		border.setSize({borders.right, size.y + borders.bottom});
		border.setPosition(size.x, 0);
		target.draw(border, states);

		// Draw bottom border
		border.setSize({size.x + borders.left, borders.bottom});
		border.setPosition(-borders.left, size.y);
		target.draw(border, states);
	}
}

///////////////////////////////////////////////////////

void ItemEntryRenderer::setProperty(std::string property, const std::string& value)
{
	property = tgui::toLower(property);
	if (property == "borders")
		setBorders(tgui::Deserializer::deserialize(tgui::ObjectConverter::Type::Borders, value).getBorders());
	else if (property == "bordercolor")
		setBorderColor(tgui::Deserializer::deserialize(tgui::ObjectConverter::Type::Color, value).getColor());
	else if (property == "backgroundcolor")
		setBackgroundColor(tgui::Deserializer::deserialize(tgui::ObjectConverter::Type::Color, value).getColor());
	else if (property == "hoverbackgroundcolor")
		m_hoverBackgroundColor = tgui::Deserializer::deserialize(tgui::ObjectConverter::Type::Color, value).getColor();
	else
		WidgetRenderer::setProperty(property, value);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemEntryRenderer::setProperty(std::string property, tgui::ObjectConverter&& value)
{
	property = tgui::toLower(property);

	if (value.getType() == tgui::ObjectConverter::Type::Borders)
	{
		if (property == "borders")
			setBorders(value.getBorders());
		else
			return WidgetRenderer::setProperty(property, std::move(value));
	}
	else if (value.getType() == tgui::ObjectConverter::Type::Color)
	{
		if (property == "bordercolor")
			setBorderColor(value.getColor());
		else if (property == "backgroundcolor")
			setBackgroundColor(value.getColor());
		else if (property == "hoverbackgroundcolor")
			m_hoverBackgroundColor = value.getColor();
		else
			WidgetRenderer::setProperty(property, std::move(value));
	}
	else
		WidgetRenderer::setProperty(property, std::move(value));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tgui::ObjectConverter ItemEntryRenderer::getProperty(std::string property) const
{
	property = tgui::toLower(property);

	if (property == "borders")
		return m_borders;
	else if (property == "bordercolor")
		return m_borderColor;
	else if (property == "backgroundcolor")
		return m_entry->m_backgroundColor;
	else if (property == "hoverbackgroundcolor")
		return m_hoverBackgroundColor;
	else
		return WidgetRenderer::getProperty(property);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, tgui::ObjectConverter> ItemEntryRenderer::getPropertyValuePairs() const
{
	auto pairs = tgui::WidgetRenderer::getPropertyValuePairs();
	pairs["BackgroundColor"] = m_entry->m_backgroundColor;
	pairs["BorderColor"] = m_borderColor;
	pairs["HoverBackgroundColor"] = m_hoverBackgroundColor;
	pairs["Borders"] = m_borders;
	return pairs;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemEntryRenderer::setBackgroundColor(const tgui::Color& color)
{
	m_entry->setBackgroundColor(color);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemEntryRenderer::setBorderColor(const tgui::Color& color)
{
	m_borderColor = color;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<tgui::WidgetRenderer> ItemEntryRenderer::clone(tgui::Widget* widget)
{
	auto renderer = std::make_shared<ItemEntryRenderer>(*this);
	renderer->m_entry = static_cast<ItemEntry*>(widget);
	return std::dynamic_pointer_cast<tgui::WidgetRenderer>(renderer);
}

///////////////////////////////////////////////////////
