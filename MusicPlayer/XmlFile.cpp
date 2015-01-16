
#include "XmlFile.h"

//
void	CXmlElement::GetText(CXmlString& value)
{
	_bstr_t		bstr;
	(*this)->get_text(bstr.GetAddress());
	value = bstr;
}

//
void	CXmlElement::SetText(CXmlString& value)
{
	_bstr_t		bstr(value.c_str());
	(*this)->put_text(bstr.GetBSTR());
}

//
void	CXmlElement::GetElementByName( CXmlString name, std::vector<CXmlElement>& pElement )
{
	pElement.clear();

	CXmlNodeList	pNodeList;
	HRESULT	hResult = (*this)->getElementsByTagName( _bstr_t(name.c_str()), &pNodeList );
	if( SUCCEEDED(hResult) )
	{
		long count = 0;
		pNodeList->get_length( &count );
		if(count > 0)
		{
			pElement.resize(count);
			for (int i = 0; i < count; i ++)
			{
				IXMLDOMNode* pNode;
				pNodeList->get_item( i, (IXMLDOMNode**)&pNode );
				pNode->QueryInterface( IID_IXMLDOMElement, (void**)&(pElement[i]) );
				pNode->Release( );
			}
		}
	}
}

//
void	CXmlElement::GetElementByName( CXmlString name, CXmlElement& pElement )
{
	this->GetElementByName( name, 0, pElement );
}

//
void	CXmlElement::GetElementByName( CXmlString name, int index, CXmlElement& pElement )
{
	if(pElement != NULL){ pElement.Release(); }

	_com_ptr_t<_com_IIID<IXMLDOMNodeList, &__uuidof(IXMLDOMNodeList)>> pNodeList;
	HRESULT	hResult = (*this)->getElementsByTagName( _bstr_t(name.c_str()), &pNodeList );
	if( SUCCEEDED(hResult) )
	{
		long count = 0;
		pNodeList->get_length( &count );
		if( count > index )
		{
			IXMLDOMNode* pNode;
			pNodeList->get_item( index, (IXMLDOMNode**)&pNode );
			pNode->QueryInterface( IID_IXMLDOMElement, (void**)&pElement );
			pNode->Release( );
		}
	}
}

//
void	CXmlElement::GetElementByIndex( int index, CXmlElement& pElement )
{
	if(pElement != NULL){ pElement.Release(); }

	_com_ptr_t<_com_IIID<IXMLDOMNodeList, &__uuidof(IXMLDOMNodeList)>> pNodeList;
	HRESULT	hResult = (*this)->get_childNodes( &pNodeList );
	if( SUCCEEDED(hResult) )
	{
		long count = 0;
		pNodeList->get_length( &count );
		if( count > index )
		{
			IXMLDOMNode* pNode;
			pNodeList->get_item( index, (IXMLDOMNode**)&pNode );
			pNode->QueryInterface( IID_IXMLDOMElement, (void**)&pElement );
			pNode->Release( );
		}
	}
}


//
void	CXmlElement::GetAttributeByName( CXmlString name, CXmlString& value )
{
	_variant_t v;
	(*this)->getAttribute( _bstr_t( name.c_str() ), &v );
	switch( v.vt )
	{
	case VT_NULL:{ value = _T(""); break; }
	default: { value = (_bstr_t)v; break; }
	}
}

//
void	CXmlElement::GetAttributeByName( CXmlString name, int& value )
{
	_variant_t v;
	(*this)->getAttribute( _bstr_t( name.c_str() ), &v );
	switch( v.vt )
	{
	case VT_BSTR:{ value = _tstoi(_bstr_t(v)); break; }
	default:{ value = v; break; }
	}
}

//
void	CXmlElement::GetAttributeByName( CXmlString name, float& value )
{
	_variant_t v;
	(*this)->getAttribute( _bstr_t( name.c_str() ), &v );
	switch( v.vt )
	{
	case VT_BSTR:{ value = (float)_tstof(_bstr_t(v)); break; }
	default:{ value = v; break; }
	}
}

//
void	CXmlElement::GetAttributeByName( CXmlString name, long& value )
{
	_variant_t v;
	(*this)->getAttribute( _bstr_t( name.c_str() ), &v );
	switch( v.vt )
	{
	case VT_BSTR:{ value = _tstol(_bstr_t(v)); break; }
	default:{ value = v; break; }
	}
}

//
void	CXmlElement::GetAttributeByName( CXmlString name, bool& value )
{
	_variant_t v;
	(*this)->getAttribute( _bstr_t( name.c_str() ), &v );
	switch( v.vt )
	{
	case VT_BSTR:{ value = _tstoi(_bstr_t(v)) != 0 ? true : false; break; }
	default:{ value = v; break; }
	}
}

//
void	CXmlElement::GetAttributeByName( CXmlString name, short& value )
{
	_variant_t v;
	(*this)->getAttribute( _bstr_t( name.c_str() ), &v );
	switch( v.vt )
	{
	case VT_BSTR:{ value = (short)_tstoi(_bstr_t(v)); break; }
	default:{ value = v; break; }
	}
}

//
void	CXmlElement::GetAttributeByName( CXmlString name, unsigned short& value )
{
	_variant_t v;
	(*this)->getAttribute( _bstr_t( name.c_str() ), &v );
	switch( v.vt )
	{
	case VT_BSTR:{ value = (unsigned short)_tstoi(_bstr_t(v)); break; }
	default:{ value = v; break; }
	}
}

void	CXmlElement::SetAttributeByName( CXmlString name, CXmlString& value )
{
	_variant_t v(value.c_str());
	(*this)->setAttribute(_bstr_t( name.c_str() ), v);
}

void	CXmlElement::SetAttributeByName( CXmlString name, int value )
{
	_variant_t v(value);
	(*this)->setAttribute(_bstr_t( name.c_str() ), v);
}

void	CXmlElement::AppendChild(CXmlElement& pElement)
{
	(*this)->appendChild(pElement, NULL);
}


//
void	CXmlDocument::AddProcessingInstruction(CXmlString target, CXmlString data)
{
	CXmlProcessingInstruction	pProcessingInstruction;
	(*this)->createProcessingInstruction( _bstr_t(target.c_str()), _bstr_t(data.c_str()), &pProcessingInstruction );
	(*this)->appendChild( pProcessingInstruction, NULL );
}

//
void	CXmlDocument::AddComment( CXmlElement& pElement, TCHAR* format, ... )
{
	TCHAR	text[100] = {0};
	va_list		v;
	va_start( v, format );
	_vsntprintf_s( text, 100, format, v );
	va_end( v );

	//Ìí¼Ó×¢ÊÍ:
	_com_ptr_t<_com_IIID<IXMLDOMComment, &__uuidof(IXMLDOMComment)>>	pComment;
	(*this)->createComment( _bstr_t(text), &pComment );
	pElement->appendChild( pComment, NULL );
}

void	CXmlDocument::AddComment( TCHAR* format, ... )
{
	TCHAR	text[100] = {0};
	va_list		v;
	va_start( v, format );
	_vsntprintf_s( text, 100, format, v );
	va_end( v );

	//Ìí¼Ó×¢ÊÍ:
	_com_ptr_t<_com_IIID<IXMLDOMComment, &__uuidof(IXMLDOMComment)>>	pComment;
	(*this)->createComment( _bstr_t(text), &pComment );
	(*this)->appendChild( pComment, NULL );
}

//
void	CXmlDocument::GetElementByDoc( CXmlElement& pElement )
{
	(*this)->get_documentElement( &pElement );
}

void	CXmlDocument::SetElementByDoc( CXmlElement& pElement )
{
	(*this)->appendChild( pElement, NULL );
}

//
bool	CXmlDocument::CreateElement( CXmlString name, CXmlElement& pElement )
{
	HRESULT hResult = (*this)->createElement( _bstr_t(name.c_str()), &pElement );
	if( SUCCEEDED(hResult) )
	{
		return true;
	}
	return false;
}






//
CXmlFile::CXmlFile(void)
		:	m_bCoInitialize( false )
{
	HRESULT hResult = m_pDocument.CreateInstance( CLSID_DOMDocument );
	if( hResult == 0x800401f0 )
	{
		CoInitialize( NULL );
		hResult = m_pDocument.CreateInstance( CLSID_DOMDocument );
		m_bCoInitialize = true;
	}
}

CXmlFile::~CXmlFile(void)
{
	if( m_bCoInitialize )
	{
		m_pDocument.Release( );
		CoUninitialize( );
	}
}

//
bool	CXmlFile::Load( CXmlString xml_file )
{
	//
	_variant_t		xmlfile( xml_file.c_str( ) );
	VARIANT_BOOL	bIsSuccess(VARIANT_FALSE);
	HRESULT hResult = m_pDocument->load( xmlfile, &bIsSuccess );
	if( FAILED(hResult) || bIsSuccess == VARIANT_FALSE )
	{ return false; }

	return true;
}


//
bool	CXmlFile::Save( CXmlString xml_file )
{
	if( m_pDocument == NULL )
	{
		return false;
	}

	if( FAILED(m_pDocument->save( _variant_t(xml_file.c_str()) ) ) )
	{
		return false;
	}
	return true;
}
