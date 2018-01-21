/*
 * toggleButton.hpp
 *
 *  Created on: 25 Feb 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_TOGGLEBUTTON_HPP_
#define LIBADO_GUI_TOGGLEBUTTON_HPP_

#include <SFML/OpenGL.hpp>
#include <TGUI/TGUI.hpp>

namespace tgui{

	class ToggleButton;

	class ToggleButtonRenderer : public tgui::ButtonRenderer{
	public:
		ToggleButtonRenderer(ToggleButton* entry) : tgui::ButtonRenderer((tgui::Button*)entry) {}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		virtual std::shared_ptr<tgui::WidgetRenderer> clone(tgui::Widget* widget) override;
	protected:
		friend class ToggleButton;
	};

	class ToggleButton : public tgui::Button{
	public:
		typedef std::shared_ptr<ToggleButton> Ptr; ///< Shared widget pointer
		typedef std::shared_ptr<const ToggleButton> ConstPtr; ///< Shared constant widget pointer

		ToggleButton() : Button(){
			m_callback.widgetType = "ToggleButton";

//			addSignal<sf::String>("Pressed");

			m_renderer = std::make_shared<ToggleButtonRenderer>(this);
			reload();
		}
		std::shared_ptr<ToggleButtonRenderer> getRenderer() const
		{
			return std::static_pointer_cast<ToggleButtonRenderer>(m_renderer);
		}

		void setChecked(bool flag);
		bool isChecked();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	protected:
		friend class ToggleButtonRenderer;
	private:
		bool checked = false;
	};
}


#endif /* LIBADO_GUI_TOGGLEBUTTON_HPP_ */
