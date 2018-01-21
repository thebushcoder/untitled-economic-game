/*
 * saveFileWindow.cpp
 *
 *  Created on: 6 Nov 2016
 *      Author: linbob
 */

#include "saveFileWindow.hpp"

void SaveFileWindow::init(tgui::Theme::Ptr theme, std::string instPath, bool isLoading){
	connect("closed", [this]{
		this->hide();
	});

	rootPath = getenv("HOME");
	rootPath += "/linbobGame/";

	instancePath = instPath;

	int entryH = 24;

	tgui::ListBox::Ptr listBox = theme->load("ListBox");
	listBox->setSize(getSize().x, getSize().y * 0.8);
	listBox->setItemHeight(entryH);
	add(listBox, "file_list");

	tgui::Scrollbar::Ptr scrollbar = theme->load("Scrollbar");

	listBox->setScrollbar(scrollbar);
	listBox->setAutoScroll(false);

	if(isLoading){
		tgui::Button::Ptr load = theme->load("Button");
		load->setText("Load");
		load->setSize(48, entryH);
		load->setPosition(getSize().x * 0.2, getSize().y - (load->getSize().y + 4));
		add(load, "load");

//		load->connect("Pressed", [load, listBox, this]{
//			if(listBox->getSelectedItem().isEmpty()) return;
//
//			this->screen->load(rootPath + instancePath + listBox->getSelectedItem());
//		});
	}else{
		tgui::EditBox::Ptr fileText = theme->load("EditBox");
		fileText->setSize(getSize().x * 0.5, entryH * 1.1);
		fileText->limitTextWidth(true);
		fileText->setPosition(getSize().x * 0.25, listBox->getSize().y + 4);
		add(fileText, "save_text");

//		fileText->connect("Focused", [screen]{
//			screen->getInput().setGuiFocused(true);
//			return true;
//		});
//		fileText->connect("Unfocused", [screen]{
//			screen->getInput().setGuiFocused(false);
//			return true;
//		});

		tgui::Button::Ptr save = theme->load("Button");
		save->setText("Save");
		save->setSize(48, entryH);
		save->setPosition(getSize().x * 0.2, getSize().y - (save->getSize().y + 4));
		add(save, "save");

//		save->connect("Pressed", [save, fileText, listBox, this]{
//			if(fileText->getText().isEmpty() && listBox->getSelectedItem().isEmpty()) return;
//
//			std::string fileName;
//			if(!fileText->getText().isEmpty()){
//				fileName = fileText->getText() + ".save";
//			}else{
//				fileName = listBox->getSelectedItem();
//			}
//
//			this->screen->save(rootPath + instancePath + fileName);
//
//			updateList();
//		});
	}

	updateList();


	hide();
}
void SaveFileWindow::updateList(){
	dirent* file;
	DIR* dir;

	//open dir
	dir = opendir(rootPath.c_str());

	//mkdir if it doesnt exist
	if(!dir){
		mkdir(rootPath.c_str(), S_IRWXU | S_IRWXG);
	}

	//open dir
	dir = opendir((rootPath + instancePath).c_str());

	//mkdir if it doesnt exist
	if(!dir){
		mkdir((rootPath + instancePath).c_str(), S_IRWXU | S_IRWXG);
		(void)closedir(dir);
		return;
	}

	std::regex fileRx("^[^\\.]+\\.save$");

	tgui::ListBox::Ptr list = std::dynamic_pointer_cast<tgui::ListBox>(get("file_list", true));

	list->removeAllItems();

	//scan dir for save files + add them to the list
	while ((file = readdir(dir)) != NULL){
		std::string name = file->d_name;
		if(std::regex_match(name, fileRx)){
			list->addItem(file->d_name);
		}
	}
	(void)closedir(dir);
}
