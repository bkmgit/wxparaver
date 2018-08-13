/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  wxparaver                                *
 *              Paraver Trace Visualization and Analysis Tool                *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

#include "timelinetreeselector.h"
#include <wx/sizer.h>
#include <map>
#include "trace.h"
#include "window.h"
#include "paravermain.h"

using std::vector;
using std::map;

#define ID_TIMELINETREE 10001

BEGIN_EVENT_TABLE( TimelineTreeSelector, wxDialog )
  EVT_TREE_ITEM_ACTIVATED( ID_TIMELINETREE, TimelineTreeSelector::OnTreeItemActivated )
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS( TimelineTreeSelector, wxDialog )

TimelineTreeSelector::TimelineTreeSelector( wxWindow* parent,
                                            wxWindowID id,
                                            const wxString& title,
                                            const std::vector<TWindowID>& windows,
                                            const Trace *currentTrace,
                                            const wxPoint& pos,
                                            const wxSize& size,
                                            long style,
                                            const wxString& name ) :
  wxDialog( parent, id, title, pos, size, style, name )
{
  CreateControls();
  fillTree( windows, currentTrace );
}

void TimelineTreeSelector::CreateControls()
{
  wxBoxSizer* itemBoxSizer = new wxBoxSizer( wxVERTICAL );
  this->SetSizer( itemBoxSizer );

  timelineTree = new wxTreeCtrl( this, ID_TIMELINETREE, wxDefaultPosition, wxDefaultSize, wxTR_HIDE_ROOT|wxTR_SINGLE|wxTR_HAS_BUTTONS|wxTR_FULL_ROW_HIGHLIGHT );
  itemBoxSizer->Add( timelineTree, 1, wxGROW );
}

void TimelineTreeSelector::OnTreeItemActivated( wxTreeEvent& event )
{
  if( timelineTree->GetItemParent( event.GetItem() ) != timelineTree->GetRootItem() )
    EndModal( wxID_OK );
}

void TimelineTreeSelector::fillTree( const std::vector<TWindowID>& windows, const Trace *currentTrace )
{
  timelineTree->SetImageList( paraverMain::myParaverMain->GetImageList() );
  
  wxTreeItemId root = timelineTree->AddRoot( wxT( "root" ) );
  wxTreeItemId currentTraceId = timelineTree->AppendItem( root,  wxString( currentTrace->getFileNameNumbered().c_str(), wxConvUTF8 ) );

  map< Trace *, wxTreeItemId > traceRoot;
  for( vector<TWindowID>::const_iterator it = windows.begin(); it != windows.end(); ++it )
  {
    if( LoadedWindows::getInstance()->getWindow( *it )->getTrace() == currentTrace )
      timelineTree->AppendItem( currentTraceId, LoadedWindows::getInstance()->getWindow( *it )->getName() );
    else
    {
      map< Trace *, wxTreeItemId >::iterator itMap = traceRoot.find( LoadedWindows::getInstance()->getWindow( *it )->getTrace() );
      wxTreeItemId insertId;
      if( itMap != traceRoot.end() )
        insertId = itMap->second;
      else
      {
        insertId = timelineTree->AppendItem( root, 
                                             wxString( LoadedWindows::getInstance()->getWindow( *it )->getTrace()->getFileNameNumbered().c_str(),
                                                       wxConvUTF8 ) );
        traceRoot[ LoadedWindows::getInstance()->getWindow( *it )->getTrace() ] = insertId;
      }
      if( !LoadedWindows::getInstance()->getWindow( *it )->isDerivedWindow() )
        timelineTree->AppendItem( insertId, LoadedWindows::getInstance()->getWindow( *it )->getName() );
    }
  }
  
  timelineTree->Expand( currentTraceId );
}