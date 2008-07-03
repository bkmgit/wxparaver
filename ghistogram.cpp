/////////////////////////////////////////////////////////////////////////////
// Name:        ghistogram.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 23 Jun 2008 11:40:03 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "ghistogram.h"
#include "histogram.h"

////@begin XPM images
////@end XPM images


/*!
 * gHistogram type definition
 */

IMPLEMENT_CLASS( gHistogram, wxFrame )


/*!
 * gHistogram event table definition
 */

BEGIN_EVENT_TABLE( gHistogram, wxFrame )

////@begin gHistogram event table entries
////@end gHistogram event table entries

END_EVENT_TABLE()


/*!
 * gHistogram constructors
 */

gHistogram::gHistogram()
{
  Init();
}

gHistogram::gHistogram( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*!
 * gHistogram creator
 */

bool gHistogram::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin gHistogram creation
  wxFrame::Create( parent, id, caption, pos, size, style );

  CreateControls();
////@end gHistogram creation
  return true;
}


/*!
 * gHistogram destructor
 */

gHistogram::~gHistogram()
{
////@begin gHistogram destruction
////@end gHistogram destruction
}


/*!
 * Member initialisation
 */

void gHistogram::Init()
{
////@begin gHistogram member initialisation
  myHistogram = NULL;
  gridHisto = NULL;
////@end gHistogram member initialisation
}


/*!
 * Control creation for gHistogram
 */

void gHistogram::CreateControls()
{    
////@begin gHistogram content construction
  gHistogram* itemFrame1 = this;

  gridHisto = new wxGrid( itemFrame1, ID_GRIDHISTO, wxDefaultPosition, itemFrame1->ConvertDialogToPixels(wxSize(200, 150)), wxHSCROLL|wxVSCROLL|wxALWAYS_SHOW_SB );
  gridHisto->SetDefaultColSize(50);
  gridHisto->SetDefaultRowSize(25);
  gridHisto->SetColLabelSize(25);
  gridHisto->SetRowLabelSize(50);
  gridHisto->CreateGrid(5, 5, wxGrid::wxGridSelectCells);

////@end gHistogram content construction
}


void gHistogram::execute()
{
  if( myHistogram == NULL )
    return;
    
  int rowLabelWidth = 0;
  wxFont labelFont = gridHisto->GetLabelFont();
  
  myHistogram->execute( myHistogram->getBeginTime(), myHistogram->getEndTime() );
  
  gridHisto->BeginBatch();
  gridHisto->DeleteCols( 0, gridHisto->GetNumberCols() );
  gridHisto->DeleteRows( 0, gridHisto->GetNumberRows() );
  gridHisto->AppendCols( myHistogram->getNumColumns() );
  gridHisto->AppendRows( myHistogram->getNumRows() );
  for( THistogramColumn iCol = 0; iCol < myHistogram->getNumColumns(); iCol++ )
  {
    gridHisto->SetColLabelValue( iCol, myHistogram->getColumnLabel( iCol ) );
    for( TObjectOrder iRow = 0; iRow < myHistogram->getNumRows(); iRow++ )
    {
      int w, h;
      gridHisto->GetTextExtent( myHistogram->getRowLabel( iCol ), &w, &h, NULL, NULL, &labelFont );
      if( rowLabelWidth == 0 || rowLabelWidth < w )
        rowLabelWidth = w;
      gridHisto->SetRowLabelValue( iRow, myHistogram->getRowLabel( iRow ) );
    }
  }
  
  gridHisto->SetRowLabelSize( rowLabelWidth );
  gridHisto->Fit();
  gridHisto->AutoSize();
  gridHisto->EndBatch();
}

/*!
 * Should we show tooltips?
 */

bool gHistogram::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap gHistogram::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin gHistogram bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end gHistogram bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon gHistogram::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin gHistogram icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end gHistogram icon retrieval
}
