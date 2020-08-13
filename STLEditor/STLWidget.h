#pragma once

#include <vector>

#include <QVTKOpenGLNativeWidget.h>
#include <qevent.h>
#include <qpoint.h>

#include <vtkRenderWindow.h>
#include <vtkNew.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkWorldPointPicker.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "ModedInteractorStyle.h"

namespace Ui { class STLWidget; };

/// <summary>
/// A widget to display an STL object.
/// </summary>
class STLWidget : public QVTKOpenGLNativeWidget
{
	Q_OBJECT

public: // Inner class
	/// <summary>
	/// A structure to contain VTK object for displaying single STL object.
	/// </summary>
	struct STLHolder
	{
	public: // Fields
		/// <summary>
		/// A STL file reader.
		/// </summary>
		vtkSTLReader* reader = nullptr;

		/// <summary>
		/// A mapper for poly data:STL object.
		/// </summary>
		vtkPolyDataMapper* mapper = nullptr;

		/// <summary>
		/// A transformation filter for STL object.
		/// </summary>
		vtkTransformPolyDataFilter* transformFilter = nullptr;

		/// <summary>
		/// The transformation matrix for filter.
		/// </summary>
		vtkTransform* transform = nullptr;

		/// <summary>
		/// An actor for mapper.
		/// </summary>
		vtkActor* actor = nullptr;

	private:
		/// <summary>
		/// The boundary coordinate information of the STL object.
		/// </summary>
		double bounds[6];

		/// <summary>
		/// A renderer to display the STL object.
		/// </summary>
		vtkRenderer* renderer = nullptr;

	public: // Constructor & Destructor
		STLHolder(QString fileName, vtkRenderer* renderer) : renderer(renderer)
		{
			assert(!fileName.isEmpty()); // must ne handled before call

			this->reader = vtkSTLReader::New();
			this->reader->SetFileName(fileName.toStdString().c_str());
			this->reader->UpdateDataObject();

			this->transform = vtkTransform::New();
			this->transform->Identity();

			this->transformFilter = vtkTransformPolyDataFilter::New();
			this->transformFilter->SetTransform(this->transform);
			this->transformFilter->SetInputConnection(this->reader->GetOutputPort());

			this->mapper = vtkPolyDataMapper::New();
			this->mapper->SetInputConnection(this->transformFilter->GetOutputPort());
			this->mapper->UpdateDataObject();

			this->actor = vtkActor::New();
			this->actor->SetMapper(this->mapper);

			this->renderer->AddActor(this->actor);
			this->renderer->GetRenderWindow()->Render();

			this->actor->GetBounds(this->bounds);
		}

		~STLHolder()
		{
			this->renderer->RemoveActor(this->actor);
			this->actor->Delete();
			this->mapper->Delete();
			this->transformFilter->Delete();
			this->transform->Delete();
			this->reader->Delete();
		}

	public:
		void reposition(int row, int column, double squareSize)
		{
			vtkNew<vtkTransform> newTransform;
			newTransform->Identity();

			newTransform->Translate(
				squareSize * column - this->bounds[0] + (squareSize - (this->bounds[1] - this->bounds[0])) / 2,
				squareSize * row - this->bounds[2] + (squareSize - (this->bounds[3] - this->bounds[2])) / 2,
				-this->bounds[4]);

			this->transform->DeepCopy(newTransform);
		}
	};

private: // UI
	/// <summary>
	/// A GUI component container.
	/// </summary>
	Ui::STLWidget* ui;

private: // Fields
	/// <summary>
	/// Indicates weather there's any object displaying.
	/// </summary>
	bool bDisplaying = false;

	/// <summary>
	/// A list of stl holders.
	/// </summary>
	std::vector<STLHolder*> holders;

	/// <summary>
	/// The length of a square that holds one STL object in it.
	/// </summary>
	double squareSize = -1;

	/// <summary>
	/// A renderer for STL object.
	/// </summary>
	vtkRenderer* renderer = nullptr;

	/// <summary>
	/// An interactor style that allows to control the object via mouse events.
	/// </summary>
	ModedInteractorStyle* mouseControlStyle = nullptr;

public: // Constructors & Destructor
	STLWidget(QWidget* parent = Q_NULLPTR);
	~STLWidget();

public: // Get & Set
	/// <summary>
	/// Open or import the STL reader to display.
	/// </summary>
	/// <param name="fileName">The name of file to open.</param>
	/// <param name="bImport">Indicates whether to open or append the new file.</param>
	/// <returns>True if you succeed to open file, false otherwise</returns>
	bool OpenSTL(QString fileName, bool bImport);

	/// <summary>
	/// Open or import the STL reader to display.
	/// </summary>
	/// <param name="fileName">The name of file to open.</param>
	/// <param name="bImport">Indicates whether to open or append the new file.</param>
	/// <returns>True if you succeed to open file, false otherwise</returns>
	bool OpenSTL(const char* fileName, bool bImport);

	/// <summary>
	/// Get the list of stl holder in std::vector<STLHolder> type.
	/// </summary>
	/// <returns>The list of stl holder.</returns>
	std::vector<STLHolder*> GetHolders(void) const;

	/// <summary>
	/// Indicates whether the widget shows any STL object.
	/// </summary>
	/// <returns>Ture if showing, false if not.</returns>
	bool HasHolder(void) const;

	/// <summary>
	/// Get the renderer.
	/// </summary>
	/// <returns>The renderer</returns>
	vtkRenderer* GetRenderer(void) const;

public: // Methods
	/// <summary>
	/// Refresh the widget, applying new changes.
	/// </summary>
	void Render(void) const;

	/// <summary>
	/// Clear all fields.
	/// </summary>
	void Clear(void);

protected: // Overriding
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
#ifndef QT_NO_WHEELEVENT
	void wheelEvent(QWheelEvent* event) override;
#endif
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void focusInEvent(QFocusEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;

Q_SIGNALS: // New event signals
	/// <summary>
	/// Event from the mouse itself and its buttons.
	/// </summary>
	/// <param name="type">The type of event.</param>
	/// <param name="button">The button of mouse.</param>
	/// <param name="location">The location where the cursor located.</param>
	void OnMouseEvent(QMouseEvent* e);

	/// <summary>
	/// Event from the keyboard/
	/// </summary>
	/// <param name="type">The type of event.</param>
	/// <param name="key">The keyboard button.</param>
	void OnKeyEvent(QKeyEvent* e);

	/// <summary>
	/// Event from the focus manager.
	/// </summary>
	/// <param name="bIsFocused">True if focused, false if not.</param>
	void OnFocusEvent(bool bIsFocused);
#ifndef QT_NO_WHEELEVENT
Q_SIGNALS:
	/// <summary>
	/// Event from the mouse wheel.
	/// </summary>
	/// <param name="event">Wheel event arguments.</param>
	void OnWheelEvent(QWheelEvent* event);
#endif
};
