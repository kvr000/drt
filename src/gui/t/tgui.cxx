/**
 * drt library
 *
 * drt multiplatform development toolkit
 *
 * ----------------------------------------------------------------------------------
 *
 * Copyright (C) 2004-2008 Zbyněk Vyškovský
 *
 * ----------------------------------------------------------------------------------
 *
 * LICENSE:
 *
 * This file is part of drt
 *
 * drt is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * drt is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with drt; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @copyright	2004-2008 Zbyněk Vyškovský
 * @link	mailto:kvr@matfyz.cz
 * @link	http://kvr.matfyz.cz/drt/
 * @license	http://www.gnu.org/licenses/lgpl.txt GNU Lesser General Public License v3
 **/

#include <stdio.h>
#include <string.h>

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/gui/Gui.hxx>
#include <dr/gui/Widget.hxx>
#include <dr/gui/Style.hxx>
#include <dr/gui/Paint.hxx>
#include <dr/gui/Label.hxx>
#include <dr/gui/Button.hxx>
#include <dr/gui/CheckBox.hxx>
#include <dr/gui/TextEdit.hxx>

#include <dr/testenv/testenv.hxx>

DR_GUI_NS_USE;
DR_TESTENV_NS_USE


#define TEST_INIT
#define TEST_WINDOW
#define TEST_RUNLOOP
#define TEST_CLOSE


class DR_EXPORT_CLS TestWidget: public Widget
{
	DR_OBJECT_DECL_SIMPLE(TestWidget, Widget);
public:
	DR_RINLINE			TestWidget(Gui *gui_)		: Widget(gui_) {}

public:
	virtual void 			paintBase(Paint *painter);

	void				button_pressed();
	void				quit_pressed();
	void				checkbox_changed(bool value);

public:
	Ref<CheckBox>			checkbox;
};

DR_OBJECT_DEF("", TestWidget, Widget);
DR_OBJECT_IMPL_SIMPLE(TestWidget);

void TestWidget::paintBase(Paint *painter)
{
	Super::paintBase(painter);
	painter->setForeground(drRGB(255, 0, 0));
	painter->drawLine(Point(0, 0), Point(geometry.getW()-1, geometry.getH()-1));
	painter->drawLine(Point(geometry.getW()-1, 0), Point(0, geometry.getH()-1));
	painter->setForeground(drRGB(0, 0, 0));
	painter->drawString(Point(80, 80), "Hello world!!!");
}

void TestWidget::button_pressed()
{
	Fatal::plog("button pressed\n");
	checkbox->setEnabled(!checkbox->isEnabled());
}

void TestWidget::checkbox_changed(bool value)
{
	Fatal::plog("checkbox changed to %d %d\n", value, checkbox->isEnabled());
}

void TestWidget::quit_pressed()
{
	gui_destroy();
}


Gui *guio = NULL;
TestWidget *main_widget = NULL;


#ifdef TEST_INIT
TESTNS(init);
void test()
{
	Fatal::plog("initializing\n");
	guio = new Gui;
	xtry {
		guio->open();
	}
	xcatch (Exception, ex) {
		Fatal::plog("failed to open GUI: %s\n", (const char *)ex->stringify().utf8());
		guio->unref();
		guio = NULL;
	}
	xend;
	Fatal::plog("initialization done\n");
}
TESTNSE(init);
#endif

#ifdef TEST_WINDOW
TESTNS(window);
void test()
{
	if (guio) {
		Ref<Label> label;
		Ref<Button> button;
		Ref<Button> quit_button;
		Ref<TextEdit> text_edit;
		Fatal::plog("window creation\n");
		main_widget = new TestWidget(guio);
		main_widget->geometry.set(20, 20, 200, 150);
		main_widget->setName("tgui");
		label.setNoref((Label *)main_widget->accStyleFactory()->createControl(Label::comp_name, main_widget));
		label->geometry.set(5, 20, 40, 20);
		label->setName("bla:");
		button.setNoref((Button *)main_widget->accStyleFactory()->createControl(Button::comp_name, main_widget));
		button->geometry.set(50, 50, 37, 75);
		button->notifyPressed(Eslot(main_widget, &TestWidget::button_pressed));
		text_edit.setNoref((TextEdit *)main_widget->accStyleFactory()->createControl(TextEdit::comp_name, main_widget));
		text_edit->geometry.set(10, 80, 100, 20);
		quit_button.setNoref((Button *)main_widget->accStyleFactory()->createControl(Button::comp_name, main_widget));
		quit_button->geometry.set(100, 60, 40, 20);
		quit_button->setName("Quit");
		quit_button->notifyPressed(Eslot(main_widget, &TestWidget::quit_pressed));
		main_widget->checkbox.setNoref((CheckBox *)main_widget->accStyleFactory()->createControl(CheckBox::comp_name, main_widget));
		main_widget->checkbox->geometry.set(50, 20, 20, 20);
		main_widget->checkbox->notifyChanged(Eslot(main_widget, &TestWidget::checkbox_changed));
		main_widget->checkbox->notifyEnabled(Eslot((Widget *)*label, &Label::setEnabled));
		main_widget->init();
		button->show(true);
		quit_button->show(true);
		main_widget->checkbox->show(true);
		main_widget->show(true);
		main_widget->checkbox->setFocus();
		Fatal::plog("window created...\n");
	}
}
TESTNSE(window);
#endif

#ifdef TEST_RUNLOOP
TESTNS(runloop);
void quiter_thread()
{
	Fatal::plog("quiting gui\n");
	guio->quited(guio);
}

void test()
{
	if (guio) {
		Fatal::plog("running gui loop\n");
		guio->runUntilQuit();
		Fatal::plog("gui returned\n");
	}
}
TESTNSE(runloop);
#endif

#ifdef TEST_CLOSE
TESTNS(close);
void test()
{
	Fatal::plog("closing gui\n");
	if (guio) {
		guio->unref();
		guio = NULL;
	}
	Fatal::plog("gui closed\n");
}
TESTNSE(close);
#endif

DR_TESTENV_MAIN()
{
	test_init();
#ifdef TEST_INIT
	TESTRUN(init);
#endif
#ifdef TEST_WINDOW
	TESTRUN(window);
#endif
#ifdef TEST_RUNLOOP
	TESTRUN(runloop);
#endif
#ifdef TEST_CLOSE
	TESTRUN(close);
#endif
	return 0;
}
