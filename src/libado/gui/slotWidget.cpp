/*
 * slotWidget.cpp
 *
 *  Created on: 21 Jun 2017
 *      Author: linbob
 */

#include "slotWidget.hpp"

void SlotEntry::updateEntry(anax::Entity i){
	SpriteComponent& s = i.getComponent<SpriteComponent>();
	get<SpriteWidget>("item_sprite")->setSprite(s.sprite.get());

	NameComponent& n = i.getComponent<NameComponent>();
	get<tgui::Label>("item_name")->setText(n.name.c_str());
	item = i;
}

void SlotEntry::clearEntry(){
	get<SpriteWidget>("item_sprite")->setSprite(nullptr);
	get<tgui::Label>("item_name")->setText("");
	setToolTip(nullptr);
}

void SlotEntry::draw(sf::RenderTarget& target, sf::RenderStates states) const{
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

void SlotEntryRenderer::setProperty(std::string property, const std::string& value)
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

void SlotEntryRenderer::setProperty(std::string property, tgui::ObjectConverter&& value)
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

tgui::ObjectConverter SlotEntryRenderer::getProperty(std::string property) const
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

std::map<std::string, tgui::ObjectConverter> SlotEntryRenderer::getPropertyValuePairs() const
{
	auto pairs = tgui::WidgetRenderer::getPropertyValuePairs();
	pairs["BackgroundColor"] = m_entry->m_backgroundColor;
	pairs["BorderColor"] = m_borderColor;
	pairs["HoverBackgroundColor"] = m_hoverBackgroundColor;
	pairs["Borders"] = m_borders;
	return pairs;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SlotEntryRenderer::setBackgroundColor(const tgui::Color& color)
{
	m_entry->setBackgroundColor(color);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SlotEntryRenderer::setBorderColor(const tgui::Color& color)
{
	m_borderColor = color;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SlotEntryRenderer::setHoverColor(const tgui::Color& color)
{
	m_hoverBackgroundColor = color;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<tgui::WidgetRenderer> SlotEntryRenderer::clone(tgui::Widget* widget)
{
	auto renderer = std::make_shared<SlotEntryRenderer>(*this);
	renderer->m_entry = static_cast<SlotEntry*>(widget);
	return std::dynamic_pointer_cast<tgui::WidgetRenderer>(renderer);
}

///////////////////////////////////////////////////////

SlotEntry::SlotEntry(){
	m_callback.widgetType = "SlotEntry";
	setBackgroundColor(sf::Color(0, 153, 51));

	connect("MouseEntered", [this]{
		if(hasItem()){
			std::static_pointer_cast<ItemTooltip>(getToolTip())->updateContents(item);
		}
	});

	m_renderer = std::make_shared<SlotEntryRenderer>(this);
	getRenderer()->setHoverColor(tgui::Color(0, 153, 200));
	reload();
}
