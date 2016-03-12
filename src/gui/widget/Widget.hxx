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

#ifndef dr__gui__Widget__hxx__
# define dr__gui__Widget__hxx__

#include <dr/String.hxx>
#include <dr/Hash.hxx>

#include <dr/gui/struct.hxx>

#include <dr/gui/Gui.hxx>
#include <dr/gui/Event.hxx>

#include <dr/Object.hxx>

DR_GUI_NS_BEGIN


class Style;
class Layout;
class Paint;
class Visual;
class Event;
class Widget_impl;


class DR_GUI_PUB Widget: public Object
{
	DR_OBJECT_DECL_SIMPLE(Widget, Object);
public:
	enum TypeFlags
	{
		TF_IsWidget		= 0x001,
		TF_IsLayout		= 0x002,
		TF_ReqDisabled		= 0x004,
		TF_Disabled		= 0x008,
		TF_Focused		= 0x010,
		TF_Unfocusable		= 0x020,
		TF_Visible		= 0x040,
		TF_PendLayout		= 0x080,
	};


	typedef struct
	{
		bool			is_active:1;
	} shell_data;

public:
	class Notifier: public Eslot_g
	{
	public:
		/* constructor */		Notifier():			Eslot_g(Null()) {}
		Notifier &			operator=(const Eslot_g &slot)	{ *(Eslot_g *)this = slot;  return *this; }
	};

public:
	DR_CONSTRUCT			Widget(Gui *gui);
	DR_CONSTRUCT			Widget(Widget *parent);

	DR_GUI_MET virtual bool		unrefed();

protected:
	virtual				~Widget();

protected:
	DR_RINLINE virtual void		gui_closed()				{}
	DR_RINLINE virtual void		gui_config()				{}
	DR_GUI_MET virtual void		gui_destroy();
	DR_GUI_MET virtual void		gui_removing();
	DR_GUI_MET virtual void		gui_removeChild(Widget *obj);
	DR_GUI_MET virtual void		gui_insertChild(Widget *obj);

public:
	virtual String			getStyleName();

public:
	virtual void			onRemovingChild(Widget *obj);

public:
	virtual int			handleEvent(Event *evt);
	virtual int			handleInherited(Event *evt);

public:
	DR_GUI_MET virtual void		setName(const String &name);
	DR_RINLINE virtual String	getName()				{ return name; }
	DR_RINLINE const String &	accName()				{ return name; }

public:
	DR_GUI_MET virtual void		onInit();
	DR_GUI_MET virtual bool		onShow(bool show);

	DR_GUI_MET virtual void		onRepaint(Event *evt);

	DR_GUI_MET virtual void		paintBase(Paint *painter);

public:
	DR_GUI_MET virtual void		init();

public:
	DR_GUI_MET bool			show(bool show);
	DR_GUI_MET void			redraw();
	DR_GUI_MET void			redraw(const Rect &rect);

	DR_GUI_MET bool			setFocus();

protected:
	virtual void			onEnabledChanged(bool enabled);

protected:
	virtual bool			loosingFocus();
	virtual bool			settingFocus();

protected:
	DR_GUI_MET void			initChilds();

public:
	DR_MINLINE bool			guiValid()				{ return gui != NULL; }
	DR_MINLINE Gui *		accGui()				{ return gui; }
	DR_MINLINE Gui_impl *		accGuiImpl()				{ return gui->impl; }

public:
	DR_RINLINE bool			isWidget()				{ return (type_flags&TF_IsWidget) != 0; }
	DR_RINLINE bool			isLayout()				{ return (type_flags&TF_IsLayout) != 0; }

public:
	DR_RINLINE Widget *		accParent()				{ return parent; }
	DR_GUI_MET Widget *		accShell();
	DR_GUI_MET Widget *		accFinalFocused();
	DR_RINLINE Visual *		accVisual()				{ return visual?visual:p_assignVisual(); }
	DR_RINLINE StyleFactory *	accStyleFactory()			{ return style_fac; }

public:
	DR_RINLINE TypeFlags		getTypeFlags()				{ return type_flags; }
	DR_RINLINE bool			isVisible()				{ return (type_flags&TF_Visible) != 0; }
	DR_RINLINE void			l_setVisible(bool vis);
	virtual void			setEnabled(bool enable);
	DR_RINLINE bool			isEnabled()				{ return (type_flags&TF_ReqDisabled) == 0; }
	DR_RINLINE bool			isReqEnabled()				{ return (type_flags&TF_Disabled) == 0; }
	DR_RINLINE void			l_setEnabled(bool enb);
	DR_RINLINE bool			isFocused()				{ return (type_flags&TF_Focused) != 0; }

public:
	virtual void			notifyEnabled(const Eslot1<void, bool> &notifier);

protected:
	Eslot_g *			findNotifier(const String &notif_id);
	Eslot1<void, bool> *		findNotifierEnabled()			{ return (Eslot1<void, bool> *)findNotifier(notify_enabled); }

protected:
	THash<String, Notifier>		notifiers;

protected:
	/** type flags */
	TypeFlags			type_flags;

	/** Gui */
	Gui *				gui;

	/** parent widget */
	Widget *			parent;

	/** childs, list, next and previous */
	Widget *			child_first, *child_last;
	Widget *			child_prev, * child_next;

	/** object's name */
	String				name;

public:
	Rect				geometry;

	Widget_impl *			main_impl;

protected:
	Widget *			focused_child;

protected:
	Ref<Visual>			visual;

public: // read-only
	Layout *			hold_layout;

public:
	Ref<StyleFactory>		style_fac;

protected:
	shell_data *			shell;

protected:
	DR_GUI_MET Visual *		p_assignVisual();

	friend class Gui;
	friend class Gui_impl;
	friend class Widget_impl;

public:
	static const String		style_name;
	static const String		notify_enabled;
};

DR_ENUM_OPS(Widget::TypeFlags);

DR_RINLINE void Widget::l_setVisible(bool vis)
{
	vis ? (type_flags |= TF_Visible) : (type_flags &= ~TF_Visible);
}

DR_RINLINE void Widget::l_setEnabled(bool enb)
{
	enb ? (type_flags &= ~TF_ReqDisabled) : (type_flags |= TF_ReqDisabled);
}


DR_GUI_NS_END

#endif
