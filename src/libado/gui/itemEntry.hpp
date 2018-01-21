/*
 * lootEntry.hpp
 *
 *  Created on: 9 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_ITEMENTRY_HPP_
#define LIBADO_GUI_ITEMENTRY_HPP_

#include <SFML/OpenGL.hpp>
#include <TGUI/TGUI.hpp>
#include <anax/Entity.hpp>

#include "spriteWidget.hpp"
#include "../entity/components/componentsCollection.hpp"
#include "../screen.hpp"

class ItemEntry;

class ItemEntryRenderer : public tgui::WidgetRenderer, public tgui::WidgetBorders, public tgui::WidgetPadding
{
public:
	ItemEntryRenderer(ItemEntry* entry) : m_entry{entry} {}

	virtual void setProperty(std::string property, const std::string& value) override;

	virtual void setProperty(std::string property, tgui::ObjectConverter&& value) override;

	virtual tgui::ObjectConverter getProperty(std::string property) const override;

	virtual std::map<std::string, tgui::ObjectConverter> getPropertyValuePairs() const override;

	void setBackgroundColor(const tgui::Color& color);

	void setBorderColor(const tgui::Color& color);

private:
	virtual std::shared_ptr<tgui::WidgetRenderer> clone(tgui::Widget* widget) override;
protected:
	ItemEntry* m_entry;

	sf::Color m_borderColor;
	sf::Color m_hoverBackgroundColor;

	friend class ItemEntry;
};

//	Displays item sprite and information
class ItemEntry : public tgui::Panel{
public:
	typedef std::shared_ptr<ItemEntry> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const ItemEntry> ConstPtr; ///< Shared constant widget pointer

	ItemEntry(){
		m_callback.widgetType = "ItemEntry";

		addSignal<sf::String>("DoubleClicked");

		m_renderer = std::make_shared<ItemEntryRenderer>(this);
		reload();
	}
	~ItemEntry(){}

	virtual void init(anax::Entity& e, tgui::Theme::Ptr theme);

	std::shared_ptr<ItemEntryRenderer> getRenderer() const{
		return std::static_pointer_cast<ItemEntryRenderer>(m_renderer);
	}
	anax::Entity& getEntity(){
		return item;
	}
protected:
	anax::Entity item;
	int baseValue = 0; //the item's value

	friend class ItemEntryRenderer;

	static void sliderChange(tgui::Label::Ptr valueLabel, tgui::EditBox::Ptr textBox, tgui::Slider::Ptr slider);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif /* LIBADO_GUI_ITEMENTRY_HPP_ */
