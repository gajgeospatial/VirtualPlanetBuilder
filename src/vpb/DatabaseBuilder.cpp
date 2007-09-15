/* -*-c++-*- VirtualPlanetBuilder - Copyright (C) 1998-2007 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#include <vpb/DatabaseBuilder>

#include <iostream>
#include <string>
#include <map>

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/io_utils>

#include <osgDB/ReadFile>
#include <osgDB/Registry>
#include <osgDB/Input>
#include <osgDB/Output>
#include <osgDB/ParameterOutput>

using namespace vpb;


class IntLookup
{
public:

    typedef int Value;

    IntLookup(Value defaultValue):
        _default(defaultValue) {}

    typedef std::map<std::string, Value> StringToValue;
    typedef std::map<Value, std::string> ValueToString;

    StringToValue   _stringToValue;
    ValueToString   _valueToString;
    
    void add(Value value, const char* str)
    {
        _stringToValue[str] = value;
        _valueToString[value] = str;
    }
    
    Value getValue(const char* str)
    {
        StringToValue::iterator itr = _stringToValue.find(str);
        if (itr==_stringToValue.end()) return _default;
        return itr->second;
    }
    
    const std::string& getString(Value value)
    {
        ValueToString::iterator itr = _valueToString.find(value);
        if (itr==_valueToString.end()) return _valueToString[_default];
        return itr->second;
    }
    
    Value _default;

};

class Serializer : public osg::Referenced
{
public:

     Serializer() {}
     
     virtual bool write(osgDB::Output&, const osg::Object&) = 0;
     virtual bool read(osgDB::Input&, osg::Object&, bool&) = 0;
};

template<typename C, typename P>
class EnumSetterAndGetter : public Serializer
{
public:

     typedef P (C::*GetterFunctionType)() const;
     typedef void (C::*SetterFunctionType)(P);

     EnumSetterAndGetter(const char* fieldName, P defaultValue, GetterFunctionType getter, SetterFunctionType setter):
        _writeOutDefaultValues(true),
        _fieldName(fieldName),
        _default(defaultValue),
        _getter(getter),
        _setter(setter),
        _lookup(defaultValue) {}
     
    void add(P value, const char* str)
    {
        _lookup.add(static_cast<IntLookup::Value>(value), str);
    }

    P getValue(const char* str)
    {
        return static_cast<P>(_lookup.getValue(str));
    }

    const std::string& getString(P value)
    {
        return _lookup.getString(static_cast<IntLookup::Value>(value));
    }

    bool write(osgDB::Output& fw, const osg::Object& obj)
    {
        const C& object = static_cast<const C&>(obj);
        if (_writeOutDefaultValues ||
            _default != (object.*_getter)())
        {
            fw.indent()<<_fieldName<<" "<<getString((object.*_getter)())<<std::endl;
        }
    }

    bool read(osgDB::Input& fr, osg::Object& obj, bool& itrAdvanced)
    {
        C& object = static_cast<C&>(obj);
        if (fr[0].matchWord(_fieldName.c_str()) && fr[1].isWord())
        {
            (object.*_setter)(getValue(fr[1].getStr()));
            fr += 2;
            itrAdvanced = true;
        }
    }

    bool               _writeOutDefaultValues;
    std::string        _fieldName;
    P                  _default;
    GetterFunctionType _getter;
    SetterFunctionType _setter;
    IntLookup          _lookup;
};


template<typename C>
class StringSetterAndGetter : public Serializer
{
public:

     typedef const std::string& P;
     typedef P (C::*GetterFunctionType)() const;
     typedef void (C::*SetterFunctionType)(P);

     StringSetterAndGetter(const char* fieldName, P defaultValue, GetterFunctionType getter, SetterFunctionType setter):
        _writeOutDefaultValues(true),
        _fieldName(fieldName),
        _default(defaultValue),
        _getter(getter),
        _setter(setter) {}
     
     bool write(osgDB::Output& fw, const osg::Object& obj)
     {
        const C& object = static_cast<const C&>(obj);
        if (_writeOutDefaultValues ||
            _default != (object.*_getter)())
        {
            fw.indent()<<_fieldName<<" "<<fw.wrapString((object.*_getter)())<<std::endl;
        }
     }

    bool read(osgDB::Input& fr, osg::Object& obj, bool& itrAdvanced)
    {
        C& object = static_cast<C&>(obj);
        if (fr[0].matchWord(_fieldName.c_str()) && (fr[1].isWord() || fr[1].isString()))
        {
            (object.*_setter)(fr[1].getStr());
            fr += 2;
            itrAdvanced = true;
        }
     }
     
     bool               _writeOutDefaultValues;
     std::string        _fieldName;
     std::string        _default;
     GetterFunctionType _getter;
     SetterFunctionType _setter;
};


template<typename C>
class IntSetterAndGetter : public Serializer
{
public:

     typedef int P;
     typedef P (C::*GetterFunctionType)() const;
     typedef void (C::*SetterFunctionType)(P);

     IntSetterAndGetter(const char* fieldName, P defaultValue, GetterFunctionType getter, SetterFunctionType setter):
        _writeOutDefaultValues(true),
        _fieldName(fieldName),
        _default(defaultValue),
        _getter(getter),
        _setter(setter) {}
     
     bool write(osgDB::Output& fw, const osg::Object& obj)
     {
        const C& object = static_cast<const C&>(obj);
        if (_writeOutDefaultValues ||
            _default != (object.*_getter)())
        {
            fw.indent()<<_fieldName<<" "<<(object.*_getter)()<<std::endl;
        }
     }

    bool read(osgDB::Input& fr, osg::Object& obj, bool& itrAdvanced)
    {
        C& object = static_cast<C&>(obj);
        if (fr[0].matchWord(_fieldName.c_str()) && fr[1].isInt())
        {
            P value;
            fr[1].getInt(value);
            (object.*_setter)(value);
            fr += 2;
            itrAdvanced = true;
        }
     }
     
     bool               _writeOutDefaultValues;
     std::string        _fieldName;
     P                  _default;
     GetterFunctionType _getter;
     SetterFunctionType _setter;
};

template<typename C>
class UIntSetterAndGetter : public Serializer
{
public:

     typedef unsigned int P;
     typedef P (C::*GetterFunctionType)() const;
     typedef void (C::*SetterFunctionType)(P);

     UIntSetterAndGetter(const char* fieldName, P defaultValue, GetterFunctionType getter, SetterFunctionType setter):
        _writeOutDefaultValues(true),
        _fieldName(fieldName),
        _default(defaultValue),
        _getter(getter),
        _setter(setter) {}
     
     bool write(osgDB::Output& fw, const osg::Object& obj)
     {
        const C& object = static_cast<const C&>(obj);
        if (_writeOutDefaultValues ||
            _default != (object.*_getter)())
        {
            fw.indent()<<_fieldName<<" "<<(object.*_getter)()<<std::endl;
        }
     }

    bool read(osgDB::Input& fr, osg::Object& obj, bool& itrAdvanced)
    {
        C& object = static_cast<C&>(obj);
        if (fr[0].matchWord(_fieldName.c_str()) && fr[1].isUInt())
        {
            P value;
            fr[1].getUInt(value);
            (object.*_setter)(value);
            fr += 2;
            itrAdvanced = true;
        }
     }
     
     bool               _writeOutDefaultValues;
     std::string        _fieldName;
     P                  _default;
     GetterFunctionType _getter;
     SetterFunctionType _setter;
};


template<typename C>
class FloatSetterAndGetter : public Serializer
{
public:

     typedef float P;
     typedef P (C::*GetterFunctionType)() const;
     typedef void (C::*SetterFunctionType)(P);

     FloatSetterAndGetter(const char* fieldName, P defaultValue, GetterFunctionType getter, SetterFunctionType setter):
        _writeOutDefaultValues(true),
        _fieldName(fieldName),
        _default(defaultValue),
        _getter(getter),
        _setter(setter) {}
     
     bool write(osgDB::Output& fw, const osg::Object& obj)
     {
        const C& object = static_cast<const C&>(obj);
        if (_writeOutDefaultValues ||
            _default != (object.*_getter)())
        {
            fw.indent()<<_fieldName<<" "<<(object.*_getter)()<<std::endl;
        }
     }

    bool read(osgDB::Input& fr, osg::Object& obj, bool& itrAdvanced)
    {
        C& object = static_cast<C&>(obj);
        if (fr[0].matchWord(_fieldName.c_str()) && fr[1].isFloat())
        {
            P value;
            fr[1].getFloat(value);
            (object.*_setter)(value);
            fr += 2;
            itrAdvanced = true;
        }
     }
     
     bool               _writeOutDefaultValues;
     std::string        _fieldName;
     P                  _default;
     GetterFunctionType _getter;
     SetterFunctionType _setter;
};

template<typename C>
class DoubleSetterAndGetter : public Serializer
{
public:

     typedef double P;
     typedef P (C::*GetterFunctionType)() const;
     typedef void (C::*SetterFunctionType)(P);

     DoubleSetterAndGetter(const char* fieldName, P defaultValue, GetterFunctionType getter, SetterFunctionType setter):
        _writeOutDefaultValues(true),
        _fieldName(fieldName),
        _default(defaultValue),
        _getter(getter),
        _setter(setter) {}
     
     bool write(osgDB::Output& fw, const osg::Object& obj)
     {
        const C& object = static_cast<const C&>(obj);
        if (_writeOutDefaultValues ||
            _default != (object.*_getter)())
        {
            fw.indent()<<_fieldName<<" "<<(object.*_getter)()<<std::endl;
        }
     }

    bool read(osgDB::Input& fr, osg::Object& obj, bool& itrAdvanced)
    {
        C& object = static_cast<C&>(obj);
        if (fr[0].matchWord(_fieldName.c_str()) && fr[1].isFloat())
        {
            P value;
            fr[1].getFloat(value);
            (object.*_setter)(value);
            fr += 2;
            itrAdvanced = true;
        }
     }
     
     bool               _writeOutDefaultValues;
     std::string        _fieldName;
     P                  _default;
     GetterFunctionType _getter;
     SetterFunctionType _setter;
};

template<typename C>
class BoolSetterAndGetter : public Serializer
{
public:

     typedef bool P;
     typedef P (C::*GetterFunctionType)() const;
     typedef void (C::*SetterFunctionType)(P);

     BoolSetterAndGetter(const char* fieldName, P defaultValue, GetterFunctionType getter, SetterFunctionType setter):
        _writeOutDefaultValues(true),
        _fieldName(fieldName),
        _default(defaultValue),
        _getter(getter),
        _setter(setter) {}
     
     bool write(osgDB::Output& fw, const osg::Object& obj)
     {
        const C& object = static_cast<const C&>(obj);
        if (_writeOutDefaultValues ||
            _default != (object.*_getter)())
        {
            fw.indent()<<_fieldName<<" "<<((object.*_getter)() ? "TRUE" : "FALSE")<<std::endl;
        }
     }

    bool read(osgDB::Input& fr, osg::Object& obj, bool& itrAdvanced)
    {
        C& object = static_cast<C&>(obj);
        if (fr[0].matchWord(_fieldName.c_str()) && fr[1].isWord())
        {
            (object.*_setter)(fr[1].matchWord("TRUE") || fr[1].matchWord("True") || fr[1].matchWord("true"));
            fr += 2;
            itrAdvanced = true;
        }
     }
     
     bool               _writeOutDefaultValues;
     std::string        _fieldName;
     P                  _default;
     GetterFunctionType _getter;
     SetterFunctionType _setter;
};

#define CREATE_STRING_SERIALIZER(CLASS,PROPERTY,PROTOTYPE) \
    new StringSetterAndGetter<CLASS>( \
    #PROPERTY, \
    PROTOTYPE.get##PROPERTY(), \
    &CLASS::get##PROPERTY, \
    &CLASS::set##PROPERTY)

#define ADD_STRING_PROPERTY(PROPERTY) _serializerList.push_back(CREATE_STRING_SERIALIZER(DatabaseBuilder,PROPERTY,prototype))


#define CREATE_UINT_SERIALIZER(CLASS,PROPERTY,PROTOTYPE) \
    new UIntSetterAndGetter<CLASS>( \
    #PROPERTY, \
    PROTOTYPE.get##PROPERTY(), \
    &CLASS::get##PROPERTY, \
    &CLASS::set##PROPERTY)

#define ADD_UINT_PROPERTY(PROPERTY) _serializerList.push_back(CREATE_UINT_SERIALIZER(DatabaseBuilder,PROPERTY,prototype))


#define CREATE_INT_SERIALIZER(CLASS,PROPERTY,PROTOTYPE) \
    new IntSetterAndGetter<CLASS>( \
    #PROPERTY, \
    PROTOTYPE.get##PROPERTY(), \
    &CLASS::get##PROPERTY, \
    &CLASS::set##PROPERTY)

#define ADD_INT_PROPERTY(PROPERTY) _serializerList.push_back(CREATE_INT_SERIALIZER(DatabaseBuilder,PROPERTY,prototype))


#define CREATE_FLOAT_SERIALIZER(CLASS,PROPERTY,PROTOTYPE) \
    new FloatSetterAndGetter<CLASS>( \
    #PROPERTY, \
    PROTOTYPE.get##PROPERTY(), \
    &CLASS::get##PROPERTY, \
    &CLASS::set##PROPERTY)

#define ADD_FLOAT_PROPERTY(PROPERTY) _serializerList.push_back(CREATE_FLOAT_SERIALIZER(DatabaseBuilder,PROPERTY,prototype))

#define CREATE_DOUBLE_SERIALIZER(CLASS,PROPERTY,PROTOTYPE) \
    new DoubleSetterAndGetter<CLASS>( \
    #PROPERTY, \
    PROTOTYPE.get##PROPERTY(), \
    &CLASS::get##PROPERTY, \
    &CLASS::set##PROPERTY)

#define ADD_DOUBLE_PROPERTY(PROPERTY) _serializerList.push_back(CREATE_DOUBLE_SERIALIZER(DatabaseBuilder,PROPERTY,prototype))


#define CREATE_BOOL_SERIALIZER(CLASS,PROPERTY,PROTOTYPE) \
    new BoolSetterAndGetter<CLASS>( \
    #PROPERTY, \
    PROTOTYPE.get##PROPERTY(), \
    &CLASS::get##PROPERTY, \
    &CLASS::set##PROPERTY)

#define ADD_BOOL_PROPERTY(PROPERTY) _serializerList.push_back(CREATE_BOOL_SERIALIZER(DatabaseBuilder,PROPERTY,prototype))


#define CREATE_ENUM_SERIALIZER(CLASS,PROPERTY,PROTOTYPE) \
    typedef EnumSetterAndGetter<DatabaseBuilder, DatabaseBuilder::PROPERTY> MySerializer;\
    osg::ref_ptr<MySerializer> serializer = new MySerializer(\
        #PROPERTY,\
        PROTOTYPE.get##PROPERTY(),\
        &CLASS::get##PROPERTY,\
        &CLASS::set##PROPERTY\
    )
    

#define ADD_ENUM_PROPERTY(PROPERTY) \
    CREATE_ENUM_SERIALIZER(DatabaseBuilder, PROPERTY, prototype); \
    _serializerList.push_back(serializer.get())

#define ADD_ENUM_VALUE(VALUE) serializer->add(DatabaseBuilder::VALUE, #VALUE)

#define ADD_ENUM_PROPERTY_TWO_VALUES(PROPERTY,VALUE1,VALUE2) \
    { \
        ADD_ENUM_PROPERTY(PROPERTY);\
        ADD_ENUM_VALUE(VALUE1);\
        ADD_ENUM_VALUE(VALUE2);\
    }

#define ADD_ENUM_PROPERTY_TWO_VALUES(PROPERTY,VALUE1,VALUE2) \
    { \
        ADD_ENUM_PROPERTY(PROPERTY);\
        ADD_ENUM_VALUE(VALUE1);\
        ADD_ENUM_VALUE(VALUE2);\
    }

#define ADD_ENUM_PROPERTY_THREE_VALUES(PROPERTY,VALUE1,VALUE2,VALUE3) \
    { \
        ADD_ENUM_PROPERTY(PROPERTY);\
        ADD_ENUM_VALUE(VALUE1);\
        ADD_ENUM_VALUE(VALUE2);\
        ADD_ENUM_VALUE(VALUE3);\
    }

#define AEV ADD_ENUM_VALUE
#define AEP ADD_ENUM_PROPERTY

class DatabaseBuilderLookUps
{
public:

    typedef std::list< osg::ref_ptr<Serializer> > SerializerList;
    SerializerList _serializerList;

    DatabaseBuilderLookUps()
    {
        DatabaseBuilder prototype;

        ADD_STRING_PROPERTY(Directory);
        ADD_STRING_PROPERTY(DestinationTileBaseName);
        ADD_STRING_PROPERTY(DestinationTileExtension);
        ADD_STRING_PROPERTY(DestinationImageExtension);
        ADD_STRING_PROPERTY(ArchiveName);
        ADD_STRING_PROPERTY(CommentString);
        
        ADD_ENUM_PROPERTY_TWO_VALUES(DatabaseType, LOD_DATABASE, PagedLOD_DATABASE)
        ADD_ENUM_PROPERTY_TWO_VALUES(GeometryType, HEIGHT_FIELD, POLYGONAL)
        ADD_ENUM_PROPERTY_THREE_VALUES(MipMappingMode, NO_MIP_MAPPING, MIP_MAPPING_HARDWARE,MIP_MAPPING_IMAGERY)

        { AEP(TextureType); AEV(RGB_24); AEV(RGBA);AEV(RGB_16);AEV(RGBA_16);AEV(COMPRESSED_TEXTURE);AEV(COMPRESSED_RGBA_TEXTURE); }

        ADD_UINT_PROPERTY(MaximumTileImageSize);
        ADD_UINT_PROPERTY(MaximumTileTerrainSize);

        ADD_FLOAT_PROPERTY(MaximumVisibleDistanceOfTopLevel);
        ADD_FLOAT_PROPERTY(RadiusToMaxVisibleDistanceRatio);
        ADD_FLOAT_PROPERTY(VerticalScale);
        ADD_FLOAT_PROPERTY(SkirtRatio);
        
        ADD_BOOL_PROPERTY(ConvertFromGeographicToGeocentric);
        ADD_BOOL_PROPERTY(UseLocalTileTransform);
        ADD_BOOL_PROPERTY(SimplifyTerrain);
        ADD_BOOL_PROPERTY(DecorateGeneratedSceneGraphWithCoordinateSystemNode);
        ADD_BOOL_PROPERTY(DecorateGeneratedSceneGraphWithMultiTextureControl);
        ADD_BOOL_PROPERTY(WriteNodeBeforeSimplification);
        
    }
    
    bool read(osgDB::Input& fr, DatabaseBuilder& db, bool& itrAdvanced)
    {
        for(SerializerList::iterator itr = _serializerList.begin();
            itr != _serializerList.end();
            ++itr)
        {
            (*itr)->read(fr,db, itrAdvanced);
        }
        return true;
    }

    bool write(osgDB::Output& fw, const DatabaseBuilder& db)
    {
        bool result = false;
        for(SerializerList::iterator itr = _serializerList.begin();
            itr != _serializerList.end();
            ++itr)
        {
            if ((*itr)->write(fw,db)) result = true;
        }
        return result;
    }

};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  DatabaseBuilder IO support
//

bool DatabaseBuilder_readLocalData(osg::Object &obj, osgDB::Input &fr);
bool DatabaseBuilder_writeLocalData(const osg::Object &obj, osgDB::Output &fw);

osgDB::TemplateRegisterDotOsgWrapperProxy<DatabaseBuilderLookUps> DatabaseBuilder_Proxy
(
    new vpb::DatabaseBuilder,
    "DatabaseBuilder",
    "DatabaseBuilder Object",
    DatabaseBuilder_readLocalData,
    DatabaseBuilder_writeLocalData
);


bool DatabaseBuilder_readLocalData(osg::Object& obj, osgDB::Input &fr)
{
    vpb::DatabaseBuilder& gt = static_cast<vpb::DatabaseBuilder&>(obj);
    bool itrAdvanced = false;
    
    DatabaseBuilder_Proxy.read(fr, gt, itrAdvanced);
    
    return itrAdvanced;
}

bool DatabaseBuilder_writeLocalData(const osg::Object& obj, osgDB::Output& fw)
{
    const vpb::DatabaseBuilder& db = static_cast<const vpb::DatabaseBuilder&>(obj);

#if 0    
    const vpb::BuildOptions& bo = static_cast<const vpb::BuildOptions&>(gt);
    fw.indent()<<"Directory "<<fw.wrapString(bo.getDirectory())<<std::endl;
    fw.indent()<<"DestinationTileBaseName "<<fw.wrapString(bo.getDestinationTileBaseName())<<std::endl;
    fw.indent()<<"DestinationTileExtension "<<fw.wrapString(bo.getDestinationTileExtension())<<std::endl;
    fw.indent()<<"DestinationImageExtension "<<fw.wrapString(bo.getDestinationImageExtension())<<std::endl;
    fw.indent()<<"ArchiveName "<<fw.wrapString(bo.getArchiveName())<<std::endl;

    fw.indent()<<"CommentString "<<fw.wrapString(bo.getCommentString())<<std::endl;

    fw.indent()<<"MaximumTileImageSize "<<bo.getMaximumTileImageSize()<<std::endl;
    fw.indent()<<"MaximumTileTerrainSize "<<bo.getMaximumTileTerrainSize()<<std::endl;
    fw.indent()<<"MaximumVisibleDistanceOfTopLevel "<<bo.getMaximumVisibleDistanceOfTopLevel()<<std::endl;
    fw.indent()<<"RadiusToMaxVisibleDistanceRatio "<<bo.getRadiusToMaxVisibleDistanceRatio()<<std::endl;
    fw.indent()<<"VerticalScale "<<bo.getVerticalScale()<<std::endl;

    fw.indent()<<"SkirtRatio "<<bo.getSkirtRatio()<<std::endl;
    fw.indent()<<"DefaultColor "<<bo.getDefaultColor()<<std::endl;

    fw.indent()<<"SimplifyTerrain "<<bo.getSimplifyTerrain()<<std::endl;
    fw.indent()<<"UseLocalTileTransform "<<bo.getUseLocalTileTransform()<<std::endl;
    
    fw.indent()<<"DecorateGeneratedSceneGraphWithCoordinateSystemNode "<<bo.getDecorateGeneratedSceneGraphWithCoordinateSystemNode()<<std::endl;
    fw.indent()<<"DecorateGeneratedSceneGraphWithMultiTextureControl "<<bo.getDecorateGeneratedSceneGraphWithMultiTextureControl()<<std::endl;
    fw.indent()<<"WriteNodeBeforeSimplification "<<bo.getWriteNodeBeforeSimplification()<<std::endl;
#endif

    DatabaseBuilder_Proxy.write(fw, db);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  DatabaseBuilder implementation
//
DatabaseBuilder::DatabaseBuilder()
{
}

DatabaseBuilder::DatabaseBuilder(const DatabaseBuilder& db,const osg::CopyOp& copyop):
    osgTerrain::TerrainTechnique(db, copyop),
    BuildOptions(db)
{
}

void DatabaseBuilder::init()
{
}

void DatabaseBuilder::update(osgUtil::UpdateVisitor*)
{
}

void DatabaseBuilder::cull(osgUtil::CullVisitor*)
{
}
