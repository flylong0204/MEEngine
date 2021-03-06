#include <MeasureTool\MeasureArea.h>
#include <MathTool\TriangleAreaVisitor.h>

MeasureArea::MeasureArea(void)
{
	m_pLine = NULL;
	m_pPolygon = NULL;
	m_pGeomGeode = NULL;
}

MeasureArea::~MeasureArea(void)
{
	if (m_pLine != NULL)
	{
		delete m_pLine;
		m_pLine = NULL;
	}
	if (m_pPolygon != NULL)
	{
		delete m_pPolygon;
		m_pPolygon = NULL;
	}
}

void MeasureArea::clickEvent(osg::Vec3 pos)
{
	if (pos == osg::Vec3())
	{
		return;
	}
	if (m_pLine != NULL)
	{
		m_pLine->addPoint(pos);
	}
	if (m_pPolygon != NULL)
	{
		m_pPolygon->addPoint(pos);
		m_pGeodeMark->addMarkSphere(pos);
		m_pCloseButton->setPosition(pos);
		float area = calcResult();
		std::wstring string = std::wstring(L"面积: ")+ 
			Conv::double2wstr(area) +  std::wstring(L"平方米");
		m_pTextInfo->setText(string);
		m_pTextInfo->setPosition(m_pPolygon->lastPoint());
		m_numVertexs = m_pPolygon->pointSize();
	}
}

void MeasureArea::moveEvent(osg::Vec3 pos)
{
	if (pos == osg::Vec3())
	{
		return;
	}
	if (m_pTipText != NULL)
	{
		if (m_isStart)
		{
			m_pTipText->setText(L"双击结束测量");
		}
		else
		{
			m_pTipText->setText(L"双击开始测量");
		}
		m_pTipText->setPosition(pos);
	}
	if (m_pPolygon != NULL)
	{
		//m_pPolygon->setPoint(m_numVertexs, pos);
	}
	if (m_pLine != NULL)
	{
		m_pLine->setPoint(m_numVertexs, pos);
		m_pLine->setPoint(m_numVertexs + 1, m_pLine->firstPoint());
	}
}

float MeasureArea::calcResult()
{
	float area = 0.0;
	TriangleAreaVisitor tav;
	area = tav.clacAllArea(m_pGeomGeode);
	return area;
}

void MeasureArea::endMeasure()
{
	m_isStart = false;
	m_isEnd = true;
	if (m_pCloseButton != NULL)
	{
		m_pTipText->setText(L"双击开始测量");
		m_pCloseButton->setNodeMask(true);
	}
	m_pLine = NULL;
	m_pPolygon = NULL;
}

void MeasureArea::init()
{
	m_pGeomGeode = new osg::Geode;
	osg::Group* root = new osg::Group;
	m_isStart = true;
	m_pPolygon = new GeometryPolygon;
	m_pPolygon->setColor(osg::Vec4(1, 1, 0, 0.5));
	m_pGeomGeode->addDrawable(m_pPolygon);

	m_pLine = new GeometryLine;
	m_pLine->setColor(osg::Vec4(0, 1, 0, 0.5));
	m_pLine->setWidth(2.0);
	m_pGeomGeode->addDrawable(m_pLine);

	root->addChild(m_pGeomGeode);

	m_pTextInfo = new PlaceNode;
	m_pTextInfo->setTextColor(osg::Vec4(0.8, 0, 0, 1));
	m_pTextInfo->setFontSize(15);
	m_pTextInfo->setTextPosition(osg::Vec3(0, -15,0));
	m_pTextInfo->setBackMode(LabelBase::FILL_BOX);
	m_pTextInfo->setBackColor(osg::Vec4(1, 1, 1, 0.5));
	root->addChild(m_pTextInfo);

	m_pCloseButton = new PlaceNode;
	m_pCloseButton->setImage(m_strImagePath);
	m_pCloseButton->setTextColor(osg::Vec4(1, 1, 1, 1));
	m_pCloseButton->setName(CLOSEBUTTON);
	m_pCloseButton->setFontSize(15);
	m_pCloseButton->setNodeMask(0x0);
	root->addChild(m_pCloseButton);

	m_pGeodeMark = new MeasureMarkSphere;
	root->addChild(m_pGeodeMark);

	root->addChild(m_pGeomGeode);
	m_pMeasGroup->addChild(root);
}
