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
		TransformActivity* transformation= (TransformActivity*)activity;
		camera->DeepCopy(transformation->getView());
	}
	break;

	case ActivityBase::EType::ZOOM:
	{
		ZoomActivity* zoom = (ZoomActivity*)activity;
		camera->Zoom(zoom->getZoomFactor());
	}
	break;

	case ActivityBase::EType::SELECT:
	{
		SelectionActivity* selection = (SelectionActivity*)activity;
		this->pick(selection->getActor());
	}
	break;
	}

	this->ui->stlWidget->Render();
}

void MainWindow::pick(vtkActor* const actor)
{
	if (this->pickedActor != nullptr) // Previously picked actor
	{
		this->pickedActor->GetProperty()->SetColor(1, 1, 1);
	}

	this->pickedActor = actor;
	if (actor != nullptr) // Currently picked actor
	{
		actor->GetProperty()->SetColor(0.75, 1, 0);
	}
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
		this->pickedActor = nullptr;
		this->manager->clear();
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

	// Transformation of when the mouse button pressed.
	static vtkCamera* initialView;

	// Location where the mouse button has pressed.
	static QPoint downPosition;

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
					downPosition = e->pos();

					vtkCamera* cam = this->ui->stlWidget->GetRenderer()->GetActiveCamera();
					initialView = vtkCamera::New();
					initialView->DeepCopy(cam);
					break;
				}
			}
			break;

		case QMouseEvent::Type::MouseButtonRelease: // Turn the command switch OFF
			if (clickedBtn == e->button())
			{
				clickedBtn = Qt::MouseButton::NoButton;

				const QPoint currentPosition = e->pos();
				if (downPosition != currentPosition) // Drag
				{
					vtkCamera* cam = this->ui->stlWidget->GetRenderer()->GetActiveCamera();

					TransformActivity* activity = new TransformActivity(initialView, cam);
					this->manager->push(activity);

					initialView->Delete();
					initialView = nullptr;

					ui->stlWidget->Render();
				}
				else // Cilck
				{
					const int* const ePos = this->ui->stlWidget->GetInteractor()->GetEventPosition();
					vtkNew<vtkPropPicker> picker;
					picker->Pick(ePos[0], ePos[1], 0, this->ui->stlWidget->GetRenderer());

					if (this->pickedActor != picker->GetActor())
					{
						SelectionActivity* activity = new SelectionActivity(this->pickedActor, picker->GetActor());
						this->manager->push(activity);

						this->pick(picker->GetActor());
					}
				}
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
				}
			}
		}
	}
}
