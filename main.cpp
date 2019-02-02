/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QWindow>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QMesh>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include "Qt3DExtras/qmetalroughmaterial.h"
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DRender/QPointLight>

#include <QPropertyAnimation>
#include "Qt3DExtras/qt3dwindow.h"
#include "Qt3DExtras/qt3dwindow.h"
#include "Qt3DExtras/qorbitcameracontroller.h"

#include <string>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  
  QSurfaceFormat format;
//  format.setVersion(4, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setSamples(1);
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  QSurfaceFormat::setDefaultFormat(format);
  
  auto pView = new Qt3DExtras::Qt3DWindow(app.primaryScreen());
  pView->setSurfaceType(QSurface::OpenGLSurface);
  pView->setFormat(format);

  pView->defaultFrameGraph()->setClearColor({QRgb(0x000000)});
  auto pInput = new Qt3DInput::QInputAspect;
  pView->registerAspect(pInput);

  auto pWidget = new QWidget;
  auto pHLayout = new QHBoxLayout(pWidget);
  pHLayout->setContentsMargins(0, 0, 0, 0);
  
  auto pVLayout = new QVBoxLayout();
  pVLayout->setAlignment(Qt::AlignTop);
  pHLayout->addLayout(pVLayout);
  
  auto pContainer = QWidget::createWindowContainer(pView);
  pHLayout->addWidget(pContainer, 1);

  pWidget->setWindowTitle(QStringLiteral("Basic shapes"));

  auto pRootEntity = new Qt3DCore::QEntity;

  auto pMaterial = new Qt3DExtras::QMetalRoughMaterial(pRootEntity);

  auto pMeshEntity = new Qt3DCore::QEntity(pRootEntity);
  auto pMesh = new Qt3DRender::QMesh(pRootEntity);
  
  pMesh->setSource(QUrl("qrc:/file.obj"));

  pMeshEntity->addComponent(pMesh);
  pMeshEntity->addComponent(pMaterial);

  auto pCamera = pView->camera();
  pCamera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 10.0f, 10000.0f);
  pCamera->setPosition({0, 0, 400.0f});
  pCamera->setViewCenter({0, 0, 0});

  Qt3DCore::QEntity *pLight = new Qt3DCore::QEntity(pRootEntity);
  Qt3DRender::QPointLight *pPointLight = new Qt3DRender::QPointLight(pLight);
  pPointLight->setColor("white");
  pPointLight->setIntensity(1);
  pLight->addComponent(pPointLight);
  Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(pLight);
  lightTransform->setTranslation(pCamera->position());
  pLight->addComponent(lightTransform);

  auto pCamController = new Qt3DExtras::QOrbitCameraController(pRootEntity);
  pCamController->setLinearSpeed( 300.0f );
  pCamController->setLookSpeed( 180.0f );
  pCamController->setCamera(pCamera);

  pView->setRootEntity(pRootEntity);
  
  pWidget->show();
  pWidget->resize({1600, 900});

  return app.exec();
}
