//  $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <SDL/SDL.h>

#include "base_gui.hpp"
#include "widget_manager.hpp"
#include "world.hpp"
#include "menu_manager.hpp"

void
BaseGUI::animateWidget(const int PREV_SELECTED_WGT, const int SELECTED_WGT)
{
	if( SELECTED_WGT != WidgetManager::WGT_NONE )
	{
		if( PREV_SELECTED_WGT != WidgetManager::WGT_NONE )
		{
			widget_manager->darken_wgt_color( PREV_SELECTED_WGT );
		}

		widget_manager->lighten_wgt_color( SELECTED_WGT );
		widget_manager->pulse_wgt( SELECTED_WGT );
	}
}

//-----------------------------------------------------------------------------
void
BaseGUI::handle(GameAction action, int value)
{
    // Skip on keypress, act on keyrelease only.
    if (value)
        return;
	
	int previous = widget_manager->get_selected_wgt();

    switch ( action )
    {
    case GA_CURSOR_LEFT:
		animateWidget(previous,
					  widget_manager->handle_left());
		
		break;
    case GA_CURSOR_RIGHT:
		animateWidget(previous,
					  widget_manager->handle_right());
		break;
    case GA_CURSOR_UP:
		animateWidget(previous,
					  widget_manager->handle_up());
		break;
    case GA_CURSOR_DOWN:
		animateWidget(previous,
					  widget_manager->handle_down());
		break;
	case GA_INC_SCROLL_SPEED:
		widget_manager->increase_scroll_speed();
		break;
	case GA_INC_SCROLL_SPEED_FAST:
		widget_manager->increase_scroll_speed(true);
		break;
	case GA_DEC_SCROLL_SPEED:
		widget_manager->decrease_scroll_speed();
		break;
	case GA_DEC_SCROLL_SPEED_FAST:
		widget_manager->decrease_scroll_speed(true);
		break;
    case GA_ENTER:
        select();
        break;
    case GA_LEAVE:
        if (menu_manager->getMenuStackSize() > 1)
        {
           if(menu_manager->isCurrentMenu(MENUID_RACEMENU))
             world->unpause();

           menu_manager->popMenu();
        }
        break;

    default:
        break;
    }   // switch
}   // handle
//-----------------------------------------------------------------------------
void
BaseGUI::inputPointer(int x, int y)
{
    const int PREV_SELECTED_WGT = widget_manager->get_selected_wgt();
    const int SELECTED_WGT = widget_manager->handle_pointer( x, y );

    if( SELECTED_WGT != WidgetManager::WGT_NONE )
    {
        if( PREV_SELECTED_WGT != WidgetManager::WGT_NONE )
        {
            widget_manager->darken_wgt_color( PREV_SELECTED_WGT );
        }

        widget_manager->lighten_wgt_color( SELECTED_WGT );
        widget_manager->pulse_wgt( SELECTED_WGT );
    }
}

//-----------------------------------------------------------------------------
void
BaseGUI::update(float dt)
{
    widget_manager->update(dt);
}   // update

//-----------------------------------------------------------------------------
void
BaseGUI::TimeToString(const double TIME, char *s)
{
    int min     = (int) floor ( TIME / 60.0 ) ;
    int sec     = (int) floor ( TIME - (double) ( 60 * min ) ) ;
    int tenths  = (int) floor ( 10.0f * (TIME - (double)(sec + 60* min)));
    sprintf ( s, "%d:%02d:%d", min,  sec,  tenths ) ;
}   // TimeToString
//-----------------------------------------------------------------------------
void
BaseGUI::inputKeyboard(SDLKey, int)
{
	// This method is not supposed to be called since BaseGUI does not
	// handle low-level keyboard input.
	assert(false);
}

//-----------------------------------------------------------------------------
/* EOF */
