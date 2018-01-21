/*
 * editorCtrlArea.cpp
 *
 *  Created on: 17 Sep 2016
 *      Author: linbob
 */

#include "editorCtrlArea.hpp"

void EditorCtrlArea::scrollYPanel(tgui::Panel::Ptr panel, tgui::Scrollbar::Ptr scroll)
{
	static int previousScrolbarValue;
    int distanceToMove = previousScrolbarValue - scroll->getValue();

    // Move all widgets that are inside the panel
    for (auto& widget : panel->getWidgets())
        widget->setPosition(widget->getPosition().x, widget->getPosition().y + distanceToMove);

    previousScrolbarValue = scroll->getValue();
}

void EditorCtrlArea::createCtrlArea(EditorScreen* screen){
	auto windowWidth = tgui::bindWidth(*screen->getManager()->getGui());
	auto windowHeight = tgui::bindHeight(*screen->getManager()->getGui());

	int panelHeight = 28 * 5;
	int childW = 500, childH = 500;
	int buttonWidth = 62, buttonHeight = 28;

	auto panel = std::make_shared<tgui::Panel>();
	panel->setSize(buttonWidth * 3.3, buttonHeight * 5);
	panel->setPosition(windowWidth - panel->getSize().x,
			windowHeight - panelHeight);

	//tile area
	auto tileWindow = std::make_shared<tgui::ChildWindow>();
	tileWindow->setSize(childW, childH);
	tileWindow->setTitle("Tile menu");
	tileWindow->hide();
	tileWindow->connect("Closed",[tileWindow]{
			tileWindow->hide();
	});
	screen->getManager()->getGui()->add(tileWindow, "tile_window");
	auto tileArea = std::make_shared<tgui::HorizontalLayout>();
	tileArea->setSize(childW, childH);
	tileWindow->add(tileArea);
	auto tileContainer = std::make_shared<tgui::Panel>();
	tileContainer->setSize(tileArea->getSize().x, tileArea->getSize().y);
	tileArea->add(tileContainer, "tileArea");

	tgui::Scrollbar::Ptr scrollbar = screen->getManager()->getTheme()->load("Scrollbar");
	scrollbar->setSize(12, tileArea->getSize().y);
	scrollbar->setArrowScrollAmount(60);
	tileArea->add(scrollbar, "scroll");
	tileArea->setRatio(scrollbar, 0.04f);

	scrollbar->connect("ValueChanged", scrollYPanel, tileContainer, scrollbar);

	//entity area
	auto entityWindow = std::make_shared<tgui::ChildWindow>();
	entityWindow->setSize(childW, childH);
	entityWindow->setTitle("Entity menu");
	entityWindow->hide();
	entityWindow->connect("Closed",[entityWindow]{
			entityWindow->hide();
	});
	screen->getManager()->getGui()->add(entityWindow, "entity_window");
	auto entityArea = std::make_shared<tgui::HorizontalLayout>();
	entityArea->setSize(childW, childH);
	entityWindow->add(entityArea);
	auto entityContainer = std::make_shared<tgui::Panel>();
	entityContainer->setSize(entityArea->getSize());
	entityArea->add(entityContainer, "entityArea");

	scrollbar = screen->getManager()->getTheme()->load("Scrollbar");
	scrollbar->setSize(12, entityArea->getSize().y);
	scrollbar->setArrowScrollAmount(60);
	entityArea->add(scrollbar, "scroll");
	entityArea->setRatio(scrollbar, 0.04f);

	scrollbar->connect("ValueChanged", scrollYPanel, entityContainer, scrollbar);

	tgui::Button::Ptr button = screen->getManager()->getTheme()->load("Button");
	button->setText("Copy");
	button->setPosition(4, 4);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [screen]{
		screen->getAreaEdit().setActive(AreaEdit::Mode::COPY);
	});
	panel->add(button);

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Cut");
	button->setPosition(buttonWidth + 8, 4);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [screen]{
		screen->getAreaEdit().setActive(AreaEdit::Mode::CUT);
	});
	panel->add(button);

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Delete");
	button->setPosition(buttonWidth * 2 + 12, 4);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [screen]{
		screen->getAreaEdit().setActive(AreaEdit::Mode::DELETE);
	});
	panel->add(button);

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Entity");
	button->setPosition(buttonWidth + 8, buttonHeight + 8);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [entityWindow]{
		entityWindow->show();
	});
	panel->add(button);

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Tiles");
	button->setPosition(buttonWidth * 2 + 12, buttonHeight + 8);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [tileWindow]{
		tileWindow->show();
	});
	panel->add(button);

	SaveFileWindow::Ptr loadWindow = std::make_shared<SaveFileWindow>();
	loadWindow->init(screen->getManager()->getTheme(), "editor/", true);
	screen->getManager()->getGui()->add(loadWindow, "loadWindow");

	loadWindow->get("load")->connect("pressed", [loadWindow, screen]{
		tgui::ListBox::Ptr list = std::dynamic_pointer_cast<tgui::ListBox>(loadWindow->get("file_list"));
		if(list->getSelectedItem().isEmpty()) return;

//		screen->setStatus(QUIT);

		screen->load(loadWindow->rootPath + loadWindow->instancePath + list->getSelectedItem());
//		EditorScreen* s = new EditorScreen();
//		s->setManager(screen->getManager());
//		s->load(loadWindow->rootPath + loadWindow->instancePath + list->getSelectedItem());
//
//		screen->getManager()->setCurrentScreen(s);
	});

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Load");
	button->setPosition(buttonWidth + 8, buttonHeight * 2 + 12);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [loadWindow]{
		loadWindow->show();
	});
	panel->add(button);

	SaveFileWindow::Ptr saveWindow = std::make_shared<SaveFileWindow>();
	saveWindow->init(screen->getManager()->getTheme(), "editor/", false);
	screen->getManager()->getGui()->add(saveWindow, "saveWindow");

	saveWindow->get("save")->connect("pressed", [saveWindow, screen]{
		tgui::EditBox::Ptr fileText = saveWindow->get<tgui::EditBox>("save_text");
		tgui::ListBox::Ptr listBox = saveWindow->get<tgui::ListBox>("file_list");
		if(fileText->getText().isEmpty() && listBox->getSelectedItem().isEmpty()) return;

		std::string fileName;
		if(!fileText->getText().isEmpty()){
			fileName = fileText->getText() + ".save";
		}else{
			fileName = listBox->getSelectedItem();
		}

		screen->save(saveWindow->rootPath + saveWindow->instancePath + fileName);

		saveWindow->updateList();
	});

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Save");
	button->setPosition(buttonWidth * 2 + 12, buttonHeight * 2 + 12);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [saveWindow]{
		saveWindow->show();
	});
	panel->add(button);

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Exit");
	button->setPosition(buttonWidth * 2 + 12, buttonHeight * 3 + 16);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [screen]{
		screen->setStatus(QUIT);
		MainMenu* s = new MainMenu();
		s->setManager(screen->getManager());
		screen->getManager()->setCurrentScreen(s);
	});
	panel->add(button);

	screen->getManager()->getGui()->add(panel, "editor_panel");
}
void EditorCtrlArea::loadTileButtons(EditorScreen* screen){
	tgui::ChildWindow::Ptr tileWindow = std::dynamic_pointer_cast<tgui::ChildWindow>(
			screen->getManager()->getGui()->get("tile_window")
			);
	tgui::Panel::Ptr tileArea = std::dynamic_pointer_cast<tgui::Panel>(tileWindow->get("tileArea", true));

	int xCount = 0, yCount = 0;
	TileMap& map = screen->getTileMap();
	TextureAtlas& atlas = screen->getAtlas();
	tgui::Theme::Ptr theme = screen->getManager()->getTheme();

	//loop tile templates
	for(std::map<int, std::unique_ptr<TileTemplate>>::const_iterator it = map.getFactory()->getTiles().begin();
			it != map.getFactory()->getTiles().end(); ++it) {

		//create sprite button
		SpriteButton::Ptr tileButton = theme->load("SpriteButton");
		tileButton->setSize(68, 68);

		tgui::Label::Ptr tooltip = theme->load("CustomTooltip");
		tooltip->setText("Move cost: " + std::to_string(it->second->getCost()));
		tileButton->setToolTip(tooltip);

		//position button
		int x, y;
		if((tileButton->getSize().x + tileButton->getSize().x * 0.1) * (xCount + 1) < tileArea->getSize().x){
			x = (tileButton->getSize().x + tileButton->getSize().x * 0.1) * xCount++;
			y = (tileButton->getSize().y + tileButton->getSize().y * 0.1) * yCount;
		}else{
			x = 0;
			y = (tileButton->getSize().y + tileButton->getSize().y * 0.1) * ++yCount;
			xCount = 1;
		}

		tileButton->setPosition(x, y);
		std::unique_ptr<sf::Sprite> s = std::move(atlas.createSprite(it->second->getSprite()));
		s->scale(1.8f, 1.8f);
		tileButton->setSprite(std::move(s));

		//attach functionality
		TileTemplate* temp = it->second.get();

		tileButton->connect("pressed", [screen, temp]{
			screen->getPlacement().setCurrent("tile");
			((TilePlaceable*)screen->getPlacement().getCurrent())->setSelection(temp);
		});
		tileArea->add(tileButton);
	}

	//set scrollbar limits
	tgui::Scrollbar::Ptr scroll = std::dynamic_pointer_cast<tgui::Scrollbar>(
			tileArea->getParent()->get("scroll", true));
	int max = yCount * 68;
	scroll->setMaximum(max);
	scroll->setLowValue(max * 0.1);
}
void EditorCtrlArea::loadEntityButtons(EditorScreen* screen){
	tgui::ChildWindow::Ptr entityWindow = std::dynamic_pointer_cast<tgui::ChildWindow>(
			screen->getManager()->getGui()->get("entity_window")
			);
	tgui::Panel::Ptr entityArea = std::dynamic_pointer_cast<tgui::Panel>(entityWindow->get("entityArea", true));

	int xCount = 0, yCount = 0;
	EntityFactory& entFactory = screen->getEntityFactory();
	TextureAtlas& atlas = screen->getAtlas();
	tgui::Theme::Ptr theme = screen->getManager()->getTheme();

	//loop entity profiles
	for(rapidjson::Value::ConstMemberIterator itr = entFactory.entityProfiles.MemberBegin();
			itr != entFactory.entityProfiles.MemberEnd(); ++itr){
		if(itr->value.HasMember("sprite")){

			std::string spriteName = itr->value["sprite"].GetString();

			//create button
			SpriteButton::Ptr entityButton = screen->getManager()->getTheme()->load("SpriteButton");
			entityButton->setSize(68, 68);

			tgui::Label::Ptr tooltip = theme->load("CustomTooltip");
			tooltip->setText(itr->name.GetString());
			entityButton->setToolTip(tooltip);

			//set position
			int x, y;
			if((entityButton->getSize().x + entityButton->getSize().x * 0.1) * (xCount + 1) < entityArea->getSize().x){
				x = (entityButton->getSize().x + entityButton->getSize().x * 0.1) * xCount++;
				y = (entityButton->getSize().y + entityButton->getSize().y * 0.1) * yCount;
			}else{
				x = 0;
				y = (entityButton->getSize().y + entityButton->getSize().y * 0.1) * ++yCount;
				xCount = 1;
			}

			entityButton->setPosition(x, y);

			std::unique_ptr<sf::Sprite> s = std::move(atlas.createSprite(spriteName));
			s->scale(1.8f, 1.8f);
			entityButton->setSprite(std::move(s));

			//add functionallity
			entityButton->connect("pressed", [itr, spriteName, screen]{
				screen->getPlacement().setCurrent("entity");
				((EntityPlaceable*)screen->getPlacement().getCurrent())->setSelection(
						itr->name.GetString(), spriteName);
			});
			entityArea->add(entityButton);
		}
	}
	//set scrollbar limits
	tgui::Scrollbar::Ptr scroll = std::dynamic_pointer_cast<tgui::Scrollbar>(
			entityArea->getParent()->get("scroll", true));
	int max = yCount * 68;
	scroll->setMaximum(max);
	scroll->setLowValue(max * 0.1);
}

