/*
 * WebServiceMap.hpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2014 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update : 23-03-2014                                                 #
#                                                                          #
# This file is part of RealOpInsight (http://RealOpInsight.com) authored   #
# by Rodrigue Chakode <rodrigue.chakode@gmail.com>                         #
#                                                                          #
# RealOpInsight is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by     #
# the Free Software Foundation, either version 3 of the License, or        #
# (at your option) any later version.                                      #
#                                                                          #
# The Software is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with RealOpInsight.  If not, see <http://www.gnu.org/licenses/>.   #
#--------------------------------------------------------------------------#
 */

#ifndef WEBSERVICEMAP_HPP
#define WEBSERVICEMAP_HPP

#include <Wt/WPaintedWidget>
#include <Wt/WContainerWidget>
#include <Wt/WPainter>
#include <Wt/WObject>
#include <Wt/WLength>
#include <Wt/WSignal>
#include <Wt/WScrollArea>
#include <Wt/WImage>
#include <QtGlobal>

struct CoreDataT;
struct NodeT;
class QString;

class WebMap : public Wt::WPaintedWidget
{
public:
  WebMap(CoreDataT* _cdata);
  virtual ~WebMap();
  void drawMap(void);

  Wt::WScrollArea* get(void) const {return m_scrollArea;}
  void updateNode(const NodeT& _node, const QString& _toolTip);
  void scaleMap(double factor);
  Wt::JSignal<double, double>& containerSizeChanged(void) {return m_containerSizeChanged;}
  Wt::Signal<void>& loaded(void) {return m_loaded;}
  void updateThumbnail(void);
  std::string thumbnailPath(void) {return m_thumbnailPath;}
  Wt::WImage* thumbnail(void) const {return m_thumbnail;}
  void emitLoaded(void) const {m_loaded.emit();}
  void setThumbnailTooltip(const std::string& tooltip) {m_thumbnail->setToolTip(tooltip);}


protected:
  void paintEvent(Wt::WPaintDevice *paintDevice);
  void layoutSizeChanged(int width, int height );

private:
  CoreDataT* m_cdata;
  double m_scaleX;
  double m_scaleY;
  std::shared_ptr<Wt::WPainter> m_painter;
  Wt::WScrollArea* m_scrollArea;
  bool m_initialLoading;
  Wt::JSignal<double, double> m_containerSizeChanged;
  Wt::Signal<void> m_loaded;
  std::string m_thumbnailPath;
  double m_translateX;
  double m_translateY;
  Wt::WImage* m_thumbnail;

  void drawNode(const NodeT& node, bool drawIcon = true);
  void drawEdge(const QString& parentId, const QString& childId);
  void createNodeLink(const NodeT& node, const Wt::WPointF& pos);
  void createExpIconLink(const NodeT& node, const Wt::WPointF& expIconPos);
  void setPreferredMethod(void);
  void setJavaScriptMember(void);
  void handleContainedSizeChanged(double w, double h);
  void expandCollapse(const QString& nodeId);
  void applyVisibilityToChild(const NodeT& node, qint8 mask);
};

#endif /* WEBSERVICEMAP_HPP */