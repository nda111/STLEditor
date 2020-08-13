#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <qmessagebox.h>
#include <qevent.h>

#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkCamera.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkMergeFilter.h>
#include <vtkPropPicker.h>
#include <vtkAreaPicker.h>
#include <vtkProperty.h>
#include <vtkProp3DCollection.h>
#include <vtkSelectPolyData.h>
#include <vtkPoints.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkPlanes.h>

#include "ui_MainWindow.h"
#include "ActivityBase.h"
#include "ActivityManager.h"
#include "TransformActivity.h"
#include "ZoomActivity.h"

namespace Ui { class MainWindow; };

/// <summary>
/// A window class that represents the main window.
/// </summary>
class MainWindow : public QMainWindow
{
	Q_OBJECT

private: // UI
	/// <summary>
	/// A GUI component container.
	/// </summary>
	Ui::MainWindow* ui;

private: // Fields
	/// <summary>
	/// The number of system arguments.
	/// </summary>
	int argc;

	/// <summary>
	/// A list of system arguments.
	/// </summary>
	char** argv;

	/// <summary>
	/// An activity manager for several actions on the program.
	/// </summary>
	ActivityManager* manager = nullptr;

	/// <summary>
	/// Poly data geometry of selected surface.
	/// </summary>
	vtkExtractPolyDataGeometry* selectionPolyData = nullptr;

	/// <summary>
	/// A poly data mapper of selected surface.
	/// </summary>
	vtkPolyDataMapper* selectionMapper = nullptr;

	/// <summary>
	/// An actor of selected surface.
	/// </summary>
	vtkActor* selectionActor = nullptr;

	/// <summary>
	// Transformation of when the mouse button pressed.
	/// </summary>
	vtkCamera* initialView;

	/// <summary>
	// Location where the mouse button has pressed.
	/// </summary>
	int downPosition[2];

public: // Constructor & Destructor
	MainWindow(int argc, char* argv[], QWidget* parent = Q_NULLPTR);
	~MainWindow();

protected: // Overriding
	void resizeEvent(QResizeEvent* e) override;

private: // Methods
	/// <summary>
	/// Bind events with corresponding widget.
	/// </summary>
	void bindEvents(void);

	/// <summary>
	/// Undo or redo an activity.
	/// </summary>
	/// <param name="activity">An activity to undo/redo.</param>
	void onUndoRedo(const ActivityBase* const activity);

	/// <summary>
	/// Select rectangle area on an object.
	/// </summary>
	/// <param name="p1">The first point that defines a rectangle.</param>
	/// <param name="p2">The second point that defines a rectangle.</param>
	void select(const int* const p1, const int* const p2);

public Q_SLOTS: // Slots
	/// <summary>
	/// Open an STL file at path from open file dialog.
	/// </summary>
	void openSTL(void);

	/// <summary>
	/// Handle a mouse event from STLWidget.
	/// </summary>
	/// <param name="e"The event arguments.></param>
	void OnMouseEvent(QMouseEvent* e);

	/// <summary>
	/// Handle a mouse wheel event from STLWidget.
	/// </summary>
	/// <param name="e"The event arguments.></param>
	void OnWheelEvent(QWheelEvent* e);

	/// <summary>
	/// Handle a keyboard event from STLWidget.
	/// </summary>
	/// <param name="e"The event arguments.></param>
	void OnKeyEvent(QKeyEvent* e);

	/// <summary>
	/// Raise undo event when undo action button clicked.
	/// </summary>
	void raiseUndo(void);

	/// <summary>
	/// Raise redo event when redo action button clicked.
	/// </summary>
	void raiseRedo(void);
};
