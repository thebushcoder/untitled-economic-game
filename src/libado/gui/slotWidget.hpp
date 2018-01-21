/*
 * EquipmentSlot.hpp
 *
 *  Created on: 9 Jun 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_SLOTWIDGET_HPP_
#define LIBADO_GUI_SLOTWIDGET_HPP_

#include <SFML/OpenGL.hpp>
#include <TGUI/TGUI.hpp>

#include "spriteWidget.hpp"
#include "../entity/components/componentsCollection.hpp"
#include "itemTooltip.hpp"

class SlotEntry;
class EquipmentSlot;

class SlotEntryRenderer : public tgui::WidgetRenderer, public tgui::WidgetBorders, public tgui::WidgetPadding
{
public:
	SlotEntryRenderer(SlotEntry* entry) : m_entry{entry} {}

	virtual void setProperty(std::string property, const std::string& value) override;

	virtual void setProperty(std::string property, tgui::ObjectConverter&& value) override;

	virtual tgui::ObjectConverter getProperty(std::string property) const override;

	virtual std::map<std::string, tgui::ObjectConverter> getPropertyValuePairs() const override;

	void setBackgroundColor(const tgui::Color& color);

	void setBorderColor(const tgui::Color& color);

	void setHoverColor(const tgui::Color& color);

private:
	virtual std::shared_ptr<tgui::WidgetRenderer> clone(tgui::Widget* widget) override;
protected:
	SlotEntry* m_entry;

	sf::Color m_borderColor;
	sf::Color m_hoverBackgroundColor;

	friend class SlotEntry;
};

class SlotEntry : public tgui::Panel{
public:
	typedef std::shared_ptr<SlotEntry> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const SlotEntry> ConstPtr;

	SlotEntry();

	void updateEntry(anax::Entity item);
	void clearEntry();

	bool hasItem(){
		return !get<tgui::Label>("item_name")->getText().isEmpty();
	}

	std::shared_ptr<SlotEntryRenderer> getRenderer() const{
		return std::static_pointer_cast<SlotEntryRenderer>(m_renderer);
	}
protected:
	friend class SlotEntryRenderer;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	anax::Entity item;
};

/////////////////////////////////////////////////////////////

class SlotWidget : public tgui::Panel{
public:
	typedef std::shared_ptr<SlotWidget> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const SlotWidget> ConstPtr;

	SlotWidget(std::string l, Item::Type it, tgui::Theme::Ptr t) : type(it), theme(t){
		setBackgroundColor(sf::Color(255, 255, 255));

		tgui::Label::Ptr label = theme->load("Label");
		label->setText(l);
		label->setTextSize(16);
		label->getRenderer()->setBackgroundColor(tgui::Color(255, 128, 128));
		label->setTextColor(tgui::Color(0, 0, 0));
		add(label, "slot_label");

		SlotEntry::Ptr entry = std::make_shared<SlotEntry>();
		add(entry, "slot_entry");
	}
	void init(){
		int w = getSize().x - 4;
		tgui::Label::Ptr label = get<tgui::Label>("slot_label");
		label->getRenderer()->setPadding(4, 2);
		label->setSize(w, getSize().y * 0.40);
		label->setPosition(2, 2);
		SlotEntry::Ptr entry = getEntry();
		entry->getRenderer()->setPadding(4, 2);
		entry->setSize(w, getSize().y * 0.56);
		entry->setPosition(2, getSize().y * 0.42);

		SpriteWidget::Ptr sprite = std::make_shared<SpriteWidget>();
//		sprite->setPosition();
		sprite->setSize(getSize().y * 0.55, getSize().y * 0.55);
		entry->add(sprite, "item_sprite");

		tgui::Label::Ptr name = theme->load("Label");
		name->setPosition(sprite->getSize().x + 4, 0);
		name->setSize(entry->getSize().x - (sprite->getSize().x + 4), getSize().y * 0.56);
		name->setTextSize(16);
		entry->add(name, "item_name");
	}
	Item::Type getType(){
		return type;
	}
	SlotEntry::Ptr getEntry(){
		return get<SlotEntry>("slot_entry");
	}
private:
	Item::Type type;
	tgui::Theme::Ptr theme = nullptr; //needed to create tooltip
};

#endif /* LIBADO_GUI_SLOTWIDGET_HPP_ */
