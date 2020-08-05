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
#include <vtkProperty.h>

#include "ui_MainWindow.h"
#include "ActivityBase.h"
#include "ActivityManager.h"
#include "TransformActivity.h"
#include "ZoomActivity.h"
#include "SelectionActivity.h"

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
	/// The actor of last picked object.
	/// </summary>
	vtkActor* pickedActor = nullptr;

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
	/// Pick an object on the screen.
	/// </summary>
	/// <param name="screenX">A new selected actor which might be null.</param>
	void pick(vtkActor* const actor);

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
