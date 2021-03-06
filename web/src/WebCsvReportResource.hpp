#ifndef WEBCSVREPORTRESOURCE_HPP
#define WEBCSVREPORTRESOURCE_HPP

#include "dbo/src/DbSession.hpp"
#include <Wt/WResource>
#include <Wt/WAnchor>

class WebCsvExportResource : public Wt::WResource
{
public:
  WebCsvExportResource(void);
  ~WebCsvExportResource(){ beingDeleted(); }
  void updateData(const std::string& viewName, const QosDataList& qosData);
  void setExportFileName(void);

  virtual void handleRequest(const Wt::Http::Request&, Wt::Http::Response& response);


private:
  QosDataList m_qosData;
  std::string m_viewName;
};



class WebCsvExportIcon : public Wt::WAnchor
{
public:
  WebCsvExportIcon(void);
  void updateData(const std::string& viewName, const QosDataList& qosData);

private:
  WebCsvExportResource m_csvResource;
};


#endif // WEBCSVREPORTRESOURCE_HPP
