#include "MainWindow.h"

class MainWindow;

MainWindow::MainWindow(int argc, char* argv[], QWidget* parent) : QMainWindow(parent), argc(argc), argv(argv)
{
	this->ui = new Ui::MainWindow();
	this->ui->setupUi(this);

	this->manager = new ActivityManager;

	bindEvents();

	if (argc > 1)
	{
		if (QFile::exists(argv[1]))
		{
			this->ui->stlWidget->OpenSTL(QString::fromStdString(std::string(argv[1])), false);
		}
		else
		{
			MessageBox((HWND)this->windowHandle(), L"File not found.", L"Error", 0);
			exit(0);
		}
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
	// Adjust STL viewer size to fit in its parent
	QSize size = e->size();
	size.setHeight(size.height() - ui->menuBar->geometry().height());
	ui->stlWidget->resize(size);
}

void MainWindow::bindEvents(void)
{
	// Open button clicked
	connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openSTL()));

	// Undo/Redo button clicked
	connect(ui->actionUndo, SIGNAL(triggered(bool)), this, SLOT(raiseUndo()));
	connect(ui->actionRedo, SIGNAL(triggered(bool)), this, SLOT(raiseRedo()));

	// STL Mouse event
	connect(ui->stlWidget, SIGNAL(OnMouseEvent(QMouseEvent*)), SLOT(OnMouseEvent(QMouseEvent*)));

	// STL Wheel event
	connect(ui->stlWidget, SIGNAL(OnWheelEvent(QWheelEvent*)), SLOT(OnWheelEvent(QWheelEvent*)));

	// STL Keyboar devent
	connect(ui->stlWidget, SIGNAL(OnKeyEvent(QKeyEvent*)), SLOT(OnKeyEvent(QKeyEvent*)));
}

void MainWindow::raiseUndo(void)
{
	ActivityBase* activity = manager->undo();
	if (activity != nullptr)
	{
		onUndoRedo(activity);
		activity->inverse();
	}
}

void MainWindow::raiseRedo(void)
{
	ActivityBase* activity = manager->redo();
	if (activity != nullptr)
	{
		onUndoRedo(activity);
		activity->inverse();
	}
}

void MainWindow::onUndoRedo(const ActivityBase* const activity)
{
	vtkCamera* const camera = this->ui->stlWidget->GetRenderer()->GetActiveCamera();

	switch (activity->getType())
	{
	case ActivityBase::EType::TRANSFORMATION:
	{
		TransformActivity* transformation = (TransformActivity*)activity;
		camera->DeepCopy(transformation->getView());
	}
	break;

	case ActivityBase::EType::ZOOM:
	{
		ZoomActivity* zoom = (ZoomActivity*)activity;
		camera->Zoom(zoom->getZoomFactor());
	}
	break;
	}

	this->ui->stlWidget->Render();
}

void MainWindow::select(const int* const p1, const int* const p2)
{
	if (this->selectionPolyData != nullptr)
	{
		this->ui->stlWidget->GetRenderer()->RemoveActor(this->selectionActor);

		this->selectionPolyData->Delete();
		this->selectionMapper->Delete();
		this->selectionActor->Delete();
	}

	vtkNew<vtkAreaPicker> picker;
	picker->AreaPick(p1[0], p1[1], p2[0], p2[1], this->ui->stlWidget->GetRenderer());
	vtkActor* const actor = picker->GetActor();
	STLWidget::STLHolder* const holder = this->ui->stlWidget->GetHolderByActor(actor);

	if (holder != nullptr)
	{
		vtkPlanes* const frustum = picker->GetFrustum();

		this->selectionPolyData = vtkExtractPolyDataGeometry::New();
		this->selectionPolyData->SetInputConnection(holder->transformFilter->GetOutputPort());
		this->selectionPolyData->SetImplicitFunction(frustum);
		this->selectionPolyData->SetExtractBoundaryCells(false);
		this->selectionPolyData->SetExtractInside(true);
		this->selectionPolyData->Update();

		if (this->selectionPolyData->GetOutput()->GetNumberOfCells() != 0)
		{
			this->selectionMapper = vtkPolyDataMapper::New();
			this->selectionMapper->SetInputDataObject(this->selectionPolyData->GetOutput());
			this->selectionMapper->ScalarVisibilityOff();

			this->selectionActor = vtkActor::New();
			this->selectionActor->SetMapper(this->selectionMapper);
			this->selectionActor->GetProperty()->SetColor(0.75, 1, 0);
			this->selectionActor->GetProperty()->SetPointSize(5);

			this->ui->stlWidget->GetRenderer()->AddActor(this->selectionActor);
		}
		else if (this->selectionMapper != nullptr)
		{
			this->selectionMapper->Delete();

			this->selectionPolyData = nullptr;
			this->selectionMapper = nullptr;
			this->selectionActor = nullptr;
		}
	}
	else
	{
		this->selectionPolyData = nullptr;
		this->selectionMapper = nullptr;
		this->selectionActor = nullptr;
	}

	this->ui->stlWidget->Render();
}

void MainWindow::openSTL()
{
	static bool openState = false;

	QMessageBox::StandardButton reply = QMessageBox::Yes;
	if (openState)
	{
		reply = QMessageBox::question(this,
			"Open", "Do you want to close previous file(s)?",
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	}

	QString fileName = "";
	switch (reply)
	{
	case QMessageBox::Yes: // Open
		fileName = QFileDialog::getOpenFileName(this, "Open", QDir::currentPath(), "Stereolithography (*.STL)");
		this->manager->clear();

		if (this->selectionPolyData != nullptr)
		{
			this->ui->stlWidget->GetRenderer()->RemoveActor(this->selectionActor);

			this->selectionPolyData->Delete();
			this->selectionMapper->Delete();
			this->selectionActor->Delete();

			this->selectionPolyData = nullptr;
			this->selectionMapper = nullptr;
			this->selectionActor = nullptr;
		}
		break;

	case QMessageBox::No: // Import
		fileName = QFileDialog::getOpenFileName(this, "Import", QDir::currentPath(), "Stereolithography (*.STL)");
		break;

	case QMessageBox::Cancel: // Just cancelled
	default:
		break;
	}

	if (!fileName.isEmpty())
	{
		this->ui->stlWidget->OpenSTL(fileName, reply == QMessageBox::No);
		openState = true;
	}
}

void MainWindow::OnMouseEvent(QMouseEvent* e)
{
	static const double ROTATION_STEP = 100.0;

	static Qt::MouseButton clickedBtn = Qt::MouseButton::NoButton;

	if (this->ui->stlWidget->HasHolder())
	{
		switch (e->type())
		{
		case QMouseEvent::Type::MouseButtonPress: // Turn the command switch ON
			if (clickedBtn == Qt::MouseButton::NoButton)
			{
				switch (e->button())
				{
				case Qt::MouseButton::LeftButton: // Translation
				case Qt::MouseButton::RightButton: // Rotation
					clickedBtn = e->button();
					this->ui->stlWidget->GetInteractor()->GetEventPosition(this->downPosition);

					vtkCamera* cam = this->ui->stlWidget->GetRenderer()->GetActiveCamera();
					this->initialView = vtkCamera::New();
					this->initialView->DeepCopy(cam);
					break;
				}
			}
			break;

		case QMouseEvent::Type::MouseButtonRelease: // Turn the command switch OFF
			if (clickedBtn == e->button())
			{
				if (this->ui->stlWidget->GetMode() == ModedInteractorStyle::EMode::SELECTION)
				{
					if (clickedBtn == Qt::MouseButton::LeftButton)
					{
						const int* const pos = this->ui->stlWidget->GetInteractor()->GetEventPosition();
						this->select(downPosition, pos);

						this->ui->stlWidget->SetMode(ModedInteractorStyle::EMode::TRANSFORM);
					}
				}

				clickedBtn = Qt::MouseButton::NoButton;
			}
			break;
		}
	}
}

void MainWindow::OnWheelEvent(QWheelEvent* e)
{
	static const double ZOOM_STEP_INC = 1.3;
	static const double ZOOM_STEP_DEC = 1.0 / ZOOM_STEP_INC;

	if (this->ui->stlWidget->HasHolder())
	{
		const double step = e->delta() > 0 ? ZOOM_STEP_INC : ZOOM_STEP_DEC;

		ZoomActivity* const zoom = new ZoomActivity(1.0 / step);
		this->manager->push(zoom);

		ui->stlWidget->GetRenderer()->GetActiveCamera()->Zoom(step);
		ui->stlWidget->Render();
	}
}

void MainWindow::OnKeyEvent(QKeyEvent* e)
{
	// Key press event
	if (e->type() == QKeyEvent::KeyPress)
	{
		const bool bControl = (e->modifiers() & Qt::KeyboardModifier::ControlModifier) == Qt::KeyboardModifier::ControlModifier;
		const bool bShift = (e->modifiers() & Qt::KeyboardModifier::ShiftModifier) == Qt::KeyboardModifier::ShiftModifier;

		if (bControl)
		{
			// Ctrl + Shift
			if (bShift)
			{
				switch (e->key())
				{
				case Qt::Key::Key_Z: // Redo
					raiseRedo();
					break;
				}
			}
			// Ctrl
			else
			{
				switch (e->key())
				{
				case Qt::Key::Key_Z: // Undo
					raiseUndo();
					break;

				case Qt::Key::Key_O: // Open / Import
					openSTL();
					break;

				case Qt::Key::Key_S:
					if (this->selectionPolyData != nullptr)
					{
						QString fileName = QFileDialog::getSaveFileName(this, "Save", QDir::currentPath(), "Stereolithography (*.STL)");
						if (!fileName.isEmpty())
						{
							vtkNew<vtkSTLWriter> writer;
							writer->SetFileName(fileName.toStdString().c_str());
							writer->SetInputConnection(this->selectionPolyData->GetOutputPort());
							writer->Update();
						}
					}
					break;
				}
			}
		}
		// No compound key
		else
		{
			switch (e->key())
			{
			case Qt::Key::Key_T:
				this->ui->stlWidget->SetMode(ModedInteractorStyle::EMode::TRANSFORM);
				puts("Mode: TRANSFORM");
				break;

			case Qt::Key::Key_S:
				this->ui->stlWidget->SetMode(ModedInteractorStyle::EMode::SELECTION);
				puts("Mode: SELECTION");
				break;

			case Qt::Key::Key_Escape:
				if (this->selectionPolyData != nullptr)
				{
					this->ui->stlWidget->GetRenderer()->RemoveActor(this->selectionActor);

					this->selectionPolyData->Delete();
					this->selectionMapper->Delete();
					this->selectionActor->Delete();

					this->selectionPolyData = nullptr;
					this->selectionMapper = nullptr;
					this->selectionActor = nullptr;

					this->ui->stlWidget->Render();
				}
				break;
			}
		}
	}
}
