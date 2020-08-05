#include "STLWidget.h"
#include "ui_STLWidget.h"

class STLWidget;

STLWidget::STLWidget(QWidget* parent) : QVTKOpenGLNativeWidget(parent)
{
	ui = new Ui::STLWidget();
	ui->setupUi(this);

	this->renderer = vtkRenderer::New();
	this->GetRenderWindow()->AddRenderer(renderer);

	QVTKInteractor* iren = this->GetInteractor();
	iren->RemoveAllObservers();

	mouseControlStyle = new CustomInteractorStyletrackballCamera;
	this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(mouseControlStyle);
}

STLWidget::~STLWidget()
{
	Clear();
	delete ui;
}

bool STLWidget::OpenSTL(QString fileName, bool bImport = false)
{
	bool result;

	if (!fileName.isEmpty())
	{
		if (!bImport)
		{
			this->squareSize = -1;
			Clear();
		}

		STLHolder* holder = new STLHolder(fileName, this->renderer);
		this->holders.push_back(holder);
		this->bDisplaying = true;

		const double* const bounds = holder->actor->GetBounds();
		const double maxWidthDepth = std::fmax(bounds[1] - bounds[0], bounds[3] - bounds[2]) * 1.1 /* Padding */;

		if (this->squareSize < maxWidthDepth)
		{
			this->squareSize = maxWidthDepth;
		}

		const int rowColCount = (int)std::ceil(std::pow(this->holders.size(), 0.5));
		for (int row = 0; row < rowColCount; row++)
		{
			for (int col = 0; col < rowColCount; col++)
			{
				const int idx = (row * rowColCount) + col;
				if (idx == this->holders.size())
				{
					goto ROW_COL_OUT;
				}

				this->holders[idx]->reposition(row, col, this->squareSize);
			}
		}
	ROW_COL_OUT:

		this->renderer->ResetCamera();
		this->Render();

		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}

bool STLWidget::OpenSTL(const char* fileName, bool bImport)
{
	return OpenSTL(QString(fileName), bImport);
}

std::vector<STLWidget::STLHolder*> STLWidget::GetHolders(void) const
{
	return this->holders;
}

bool STLWidget::HasHolder(void) const
{
	return !this->holders.empty();
}

vtkRenderer* STLWidget::GetRenderer(void) const
{
	return this->renderer;
}

void STLWidget::Render(void) const
{
	this->renderer->GetRenderWindow()->Render();
}

void STLWidget::Clear(void)
{
	if (bDisplaying)
	{
		for (int i = 0; i < this->holders.size(); i++)
		{
			delete this->holders[i];
		}
		this->holders.clear();
		renderer->Delete();

		bDisplaying = false;
	}
}

void STLWidget::mousePressEvent(QMouseEvent* event)
{
	QVTKOpenGLNativeWidget::mousePressEvent(event);
	emit OnMouseEvent(event);
}

void STLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QVTKOpenGLNativeWidget::mouseReleaseEvent(event);
	emit OnMouseEvent(event);
}

void STLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	QVTKOpenGLNativeWidget::mouseDoubleClickEvent(event);
	emit OnMouseEvent(event);
}

void STLWidget::mouseMoveEvent(QMouseEvent* event)
{
	QVTKOpenGLNativeWidget::mouseMoveEvent(event);
	emit OnMouseEvent(event);
}

#ifndef QT_NO_WHEELEVENT
void STLWidget::wheelEvent(QWheelEvent* event)
{
	QVTKOpenGLNativeWidget::wheelEvent(event);
	emit OnWheelEvent(event);
}
#endif

void STLWidget::keyPressEvent(QKeyEvent* event)
{
	QVTKOpenGLNativeWidget::keyPressEvent(event);
	emit OnKeyEvent(event);
}

void STLWidget::keyReleaseEvent(QKeyEvent* event)
{
	QVTKOpenGLNativeWidget::keyReleaseEvent(event);
	emit OnKeyEvent(event);
}

void STLWidget::focusInEvent(QFocusEvent* event)
{
	QVTKOpenGLNativeWidget::focusInEvent(event);
	emit OnFocusEvent(true);
}

void STLWidget::focusOutEvent(QFocusEvent* event)
{
	QVTKOpenGLNativeWidget::focusOutEvent(event);
	emit OnFocusEvent(false);
}
