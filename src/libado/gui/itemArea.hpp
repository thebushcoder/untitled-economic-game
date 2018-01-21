/*
 * contextMenu.hpp
 *
 *  Created on: 4 Sep 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_ITEMAREA_HPP_
#define LIBADO_GUI_ITEMAREA_HPP_

#include <TGUI/TGUI.hpp>
#include <vector>

#include "itemEntry.hpp"

class GameScreen;

class ConfirmDialog : public tgui::ChildWindow{
public:
	typedef std::shared_ptr<ConfirmDialog> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const ConfirmDialog> ConstPtr; ///< Shared constant widget pointer

	ConfirmDialog() : tgui::ChildWindow(){}
	void init(tgui::Theme::Ptr theme);
	template <typename Func>
	void show(std::string s, Func action){
		auto l = this->get<tgui::Label>("label");
		l->setText(s);
		tgui::Button::Ptr b = get<tgui::Button>("conf_button");
		b->disconnectAll();
		b->connect("pressed", action);
		tgui::ChildWindow::show();
	}
};

/*
 * Right click context menu for ItemArea's ItemEntries
 * Presents options. Eg: use, sell, drop, equip
 */

class ItemArea;
class ItemContext : public tgui::Panel{
public:
	typedef std::shared_ptr<ItemContext> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const ItemContext> ConstPtr; ///< Shared constant widget pointer

	static const int buttonHeight = 28;

	ItemContext(GameScreen* gs, ItemArea* p);

	/*
	 * creates button and adds it to the inactive collection
	 */
	template <typename Func>
	void createButton(std::string label, std::string tag, Func func){
		tgui::Button::Ptr button = theme->load("Button");
		button->setText(label);
		button->setSize(getSize().x, buttonHeight);
		button->connect("pressed", func);
		buttons[tag] = button;
	}
	virtual void show(int x, int y, ItemEntry::Ptr w);
	/*
	 * Retrieves button fron inactive collection and adds it to the context menu
	 */
	void addButton(std::string button, int ySlot){
		buttons[button]->setPosition(0, (buttonHeight * ySlot) + (4 * ySlot));
		add(buttons[button]);
	}
	ItemEntry::Ptr getSelected(){
		return selected;
	}
	ConfirmDialog::Ptr getConfDialog(){
		return dialog;
	}
protected:
	virtual void update(sf::Time elapsedTime) override;
private:
	GameScreen* gameScreen;
	tgui::Theme::Ptr theme;
	ItemArea* parent;

	ItemEntry::Ptr selected;
	ConfirmDialog::Ptr dialog;

	std::map<std::string, tgui::Button::Ptr> buttons;
};

///////////////////////////////////

/*
 * A scrollable(V) container that lists items from an entity's inventory
 */
class ItemArea : public tgui::HorizontalLayout{
public:
	typedef std::shared_ptr<ItemArea> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const ItemArea> ConstPtr; ///< Shared constant widget pointer

	ItemArea(GameScreen* gs);
	void updateContents(anax::Entity e);
	ItemContext::Ptr getContext();
	void setSize(const tgui::Layout& width, const tgui::Layout& height){
		tgui::HorizontalLayout::setSize(width, height);
		get(0)->setSize(width, height);
		get(1)->setSize(12, height);
	}
	int getParentEntity(){
		return parentEntity;
	}
	void show();
	void hide();
	std::vector<ItemEntry::Ptr>& getMarked(){
		return marked;
	}
	tgui::Panel::Ptr getEntryArea(){
		return std::dynamic_pointer_cast<tgui::Panel>(get(0));
	}
	void removeMarked(ItemEntry::Ptr e){
		e->setBackgroundColor(sf::Color(0, 153, 51));

		marked.erase(std::remove_if(marked.begin(), marked.end(),
			[e](const ItemEntry::Ptr i) -> bool
			{
				return e.get() == i.get();
			}));
	}
	void clearMarked(){
		for(auto& m : marked){
			if(m) m->setBackgroundColor(sf::Color(0, 153, 51));
		}

		marked.clear();
	}
	void addMarked(ItemEntry::Ptr entry);
	bool isMarked(ItemEntry::Ptr entry){
		std::vector<ItemEntry::Ptr>::iterator it = std::find_if(
				marked.begin(), marked.end(), [entry](const ItemEntry::Ptr e){
			return e == entry;
		});

		return it != marked.end();
	}
	bool isMarkedEmpty(){
		return marked.empty();
	}
protected:
	static void scrollPanel(tgui::Panel::Ptr panel, tgui::Scrollbar::Ptr scroll);
private:
	int entryHeight = 62;
	GameScreen* gameScreen;
	int x, y;

	int parentEntity = -1; // EMPTY == -1
	ItemContext::Ptr context;
	std::vector<ItemEntry::Ptr> marked;
};


#endif /* LIBADO_GUI_ITEMAREA_HPP_ */
