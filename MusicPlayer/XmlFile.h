#pragma once

//
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <comdef.h>
#include <msxml2.h>
#pragma comment( lib, "msxml2.lib" )

//
typedef std::basic_string<TCHAR>	CXmlString;
typedef	_com_ptr_t<_com_IIID<IXMLDOMProcessingInstruction, &__uuidof(IXMLDOMProcessingInstruction)>>	CXmlProcessingInstruction;
typedef _com_ptr_t<_com_IIID<IXMLDOMNodeList, &__uuidof(IXMLDOMNodeList)>>								CXmlNodeList;

//
class CXmlElement : public _com_ptr_t<_com_IIID<IXMLDOMElement, &__uuidof(IXMLDOMElement)>>
{
public:
	void	GetText(CXmlString& value);
	void	SetText(CXmlString& value);

public:
	//
	void	GetElementByName( CXmlString name, std::vector<CXmlElement>& pElement );
	// ���ͬ�����ͬ��Ԫ��,��Ϊ��һ��
	void	GetElementByName( CXmlString name, CXmlElement& pElement );
	// ��ȡָ������,ָ������
	void	GetElementByName( CXmlString name, int index, CXmlElement& pElement );
	// ֻ��˳�����ȡ
	void	GetElementByIndex( int index, CXmlElement& pElement );

public:
	// ���ͬ�����ͬ��Ԫ��,��Ϊ��һ��
	void	GetAttributeByName( CXmlString name, CXmlString& value );
	void	GetAttributeByName( CXmlString name, int& value );
	void	GetAttributeByName( CXmlString name, float& value );
	void	GetAttributeByName( CXmlString name, long& value );
	void	GetAttributeByName( CXmlString name, bool& value );
	void	GetAttributeByName( CXmlString name, short& value );
	void	GetAttributeByName( CXmlString name, unsigned short& value );
	
	void	SetAttributeByName( CXmlString name, CXmlString& value );
	void	SetAttributeByName( CXmlString name, int value );

public:
	void	AppendChild(CXmlElement& pElement);
};

//
class CXmlDocument : public _com_ptr_t<_com_IIID<IXMLDOMDocument, &__uuidof(IXMLDOMDocument)>>
{
public:
	void	AddProcessingInstruction(CXmlString target, CXmlString data);
	void	AddComment( CXmlElement& pElement, TCHAR* format, ... );
	void	AddComment( TCHAR* format, ... );

	void	GetElementByDoc( CXmlElement& pElement );
	void	SetElementByDoc( CXmlElement& pElement );
	bool	CreateElement( CXmlString name, CXmlElement& pElement );
};


//
class CXmlFile
{
public:
	CXmlFile(void);
	virtual ~CXmlFile(void);

protected:
	bool				m_bCoInitialize;
	CXmlDocument		m_pDocument;

public:
	CXmlDocument&		GetDocument( ){ return m_pDocument; }

public:
	virtual bool	Load( CXmlString xml_file );
	virtual bool	Save( CXmlString xml_file );
};
