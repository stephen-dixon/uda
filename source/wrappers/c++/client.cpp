//
// Created by jholloc on 08/03/16.
//

#include "client.hpp"

#include <boost/format.hpp>

#include <client/accAPI.h>
#include <client/udaGetAPI.h>
#include <client/udaClient.h>
#include <client/udaPutAPI.h>
#include <clientserver/udaTypes.h>
#include <complex>
#include <clientserver/initStructs.h>

#include "data.hpp"
#include "result.hpp"
#include "signal.hpp"

void uda::Client::setProperty(Property prop, bool value) throw(UDAException)
{
    std::string name;

    switch (prop) {
        case PROP_DATADBLE:  name = "get_datadble"; break;
        case PROP_DIMDBLE:   name = "get_dimdble"; break;
        case PROP_TIMEDBLE:  name = "get_timedble"; break;
        case PROP_BYTES:     name = "get_bytes"; break;
        case PROP_BAD:       name = "get_bad"; break;
        case PROP_META:      name = "get_meta"; break;
        case PROP_ASIS:      name = "get_asis"; break;
        case PROP_UNCAL:     name = "get_uncal"; break;
        case PROP_NOTOFF:    name = "get_notoff"; break;
        case PROP_SYNTHETIC: name = "get_synthetic"; break;
        case PROP_SCALAR:    name = "get_scalar"; break;
        case PROP_NODIMDATA: name = "get_nodimdata"; break;
        case PROP_VERBOSE:   name = "verbose"; break;
        case PROP_DEBUG:     name = "debug"; break;
        case PROP_ALTDATA:   name = "altdata"; break;

        case PROP_TIMEOUT:
        case PROP_ALTRANK:
            throw UDAException("Cannot set boolean value for non-boolean property");

        default:
            throw UDAException("Unknown property");
    }

    value ? setIdamProperty(name.c_str()) : resetIdamProperty(name.c_str());
}

void uda::Client::setProperty(Property prop, int value) throw(UDAException)
{
    std::string name;

    switch (prop) {
        case PROP_DATADBLE:
        case PROP_DIMDBLE:
        case PROP_TIMEDBLE:
        case PROP_BYTES:
        case PROP_BAD:
        case PROP_META:
        case PROP_ASIS:
        case PROP_UNCAL:
        case PROP_NOTOFF:
        case PROP_SYNTHETIC:
        case PROP_SCALAR:
        case PROP_NODIMDATA:
        case PROP_VERBOSE:
        case PROP_DEBUG:
        case PROP_ALTDATA:
            throw UDAException("Cannot set integer value for boolean property");

        case PROP_TIMEOUT:
            name = (boost::format("timeout=%1%") % value).str();
            setIdamProperty(name.c_str());
            break;
        case PROP_ALTRANK:
            name = (boost::format("altrank=%1%") % value).str();
            setIdamProperty(name.c_str());
            break;

        default:
            throw UDAException("Unknown property");
    }
}

int uda::Client::property(Property prop) throw(UDAException)
{
    switch (prop) {
        case PROP_DATADBLE:  return getIdamProperty("get_datadble");
        case PROP_DIMDBLE:   return getIdamProperty("get_dimdble");
        case PROP_TIMEDBLE:  return getIdamProperty("get_timedble");
        case PROP_BYTES:     return getIdamProperty("get_bytes");
        case PROP_BAD:       return getIdamProperty("get_bad");
        case PROP_META:      return getIdamProperty("get_meta");
        case PROP_ASIS:      return getIdamProperty("get_asis");
        case PROP_UNCAL:     return getIdamProperty("get_uncal");
        case PROP_NOTOFF:    return getIdamProperty("get_notoff");
        case PROP_SYNTHETIC: return getIdamProperty("get_synthetic");
        case PROP_SCALAR:    return getIdamProperty("get_scalar");
        case PROP_NODIMDATA: return getIdamProperty("get_nodimdata");
        case PROP_VERBOSE:   return getIdamProperty("verbose");
        case PROP_DEBUG:     return getIdamProperty("debug");
        case PROP_ALTDATA:   return getIdamProperty("altdata");
        case PROP_TIMEOUT:   return getIdamProperty("timeout");
        case PROP_ALTRANK:   return getIdamProperty("altrank");

        default:
            throw UDAException("Unknown property");
    }
}

void uda::Client::setServerHostName(const std::string& hostName)
{
    putIdamServerHost(hostName.c_str());
}

void uda::Client::setServerPort(int portNumber)
{
    putIdamServerPort(portNumber);
}

std::string uda::Client::serverHostName()
{
    return getIdamServerHost();
}

int uda::Client::serverPort()
{
    return getIdamServerPort();
}

const uda::Result& uda::Client::get(const std::string& signalName, const std::string& dataSource) throw(UDAException)
{
    auto data = new Result(idamGetAPI(signalName.c_str(), dataSource.c_str()));

    if (data->errorCode() != OK) {
        IDAMERRORSTACK* errorstack = getIdamServerErrorStack();
        std::vector<std::string> backtrace;
        std::string msg = errorstack->nerrors > 0 ? errorstack->idamerror[0].msg : "";

        backtrace.reserve(errorstack->nerrors);
        for (unsigned int i = 0; i < errorstack->nerrors; ++i) {
            backtrace.push_back(std::string("[") + errorstack->idamerror[i].location + "]: " + errorstack->idamerror[i].msg);
        }
        delete data;

        throw UDAException(msg, backtrace);
    }

    data_.push_back(data);
    return *data;
}

static int typeIDToUDAType(const std::type_info& type)
{
    if (type == typeid(char))
        return UDA_TYPE_CHAR;
    if (type == typeid(short))
        return UDA_TYPE_SHORT;
    if (type == typeid(int))
        return UDA_TYPE_INT;
    if (type == typeid(unsigned int))
        return UDA_TYPE_UNSIGNED_INT;
    if (type == typeid(long))
        return UDA_TYPE_LONG;
    if (type == typeid(float))
        return UDA_TYPE_FLOAT;
    if (type == typeid(double))
        return UDA_TYPE_DOUBLE;
    if (type == typeid(unsigned char))
        return UDA_TYPE_UNSIGNED_CHAR;
    if (type == typeid(unsigned short))
        return UDA_TYPE_UNSIGNED_SHORT;
    if (type == typeid(unsigned long))
        return UDA_TYPE_UNSIGNED_LONG;
    if (type == typeid(long long))
        return UDA_TYPE_LONG64;
    if (type == typeid(unsigned long long))
        return UDA_TYPE_UNSIGNED_LONG64;
    if (type == typeid(std::complex<float>))
        return UDA_TYPE_COMPLEX;
    if (type == typeid(std::complex<double>))
        return UDA_TYPE_DCOMPLEX;
    if (type == typeid(char*))
        return UDA_TYPE_STRING;

    return UDA_TYPE_UNKNOWN;
}

//typedef struct PutDataBlock {
//    int data_type;
//    unsigned int rank;
//    unsigned int count;
//    int* shape;
//    const char* data;
//    int opaque_type;                // Identifies the Data Structure Type;
//    int opaque_count;               // Number of Instances of the Data Structure;
//    void* opaque_block;             // Opaque pointer to Hierarchical Data Structures
//    unsigned int blockNameLength;   // Size of the Name character string
//    char* blockName;                // Name of the Data Block
//} PUTDATA_BLOCK;

void uda::Client::put(const uda::Signal& signal)
{
    std::string filename = (boost::format("%s%06d.nc") % signal.alias() % signal.shot()).str();

    std::string signal_class;
    switch (signal.signalClass()) {
        case uda::ANALYSED:
            signal_class = "Analysed";
            break;
        case uda::RAW:
            signal_class = "Raw";
            break;
        case uda::MODELLED:
            signal_class = "Modelled";
            break;
    }

    std::string request = (boost::format("putdata::open(/create,"
                                " filename='%s',"
                                " conventions='Fusion-1.0',"
                                " class='%s',"
                                " title='%s',"
                                " shot=%d,"
                                " pass=%d,"
                                " comment='%s',"
                                " code=%s,"
                                " version=1)")
            % filename % signal_class % signal.title() % signal.shot() % signal.pass()
            % signal.comment() % signal.code()).str();

    idamPutAPI(request.c_str(), nullptr);

    PUTDATA_BLOCK pdblock{};
    initIdamPutDataBlock(&pdblock);

    const uda::Array& array = signal.array();

    pdblock.data_type = typeIDToUDAType(array.type());
    pdblock.rank = (unsigned int)array.dims().size();
    pdblock.count = (unsigned int)array.size();

    std::vector<int> shape(pdblock.rank);
    pdblock.shape = shape.data();

    pdblock.data = (char*)array.byte_data();

    idamPutAPI("", &pdblock);
}

template <typename T>
void put_scalar(const std::string& instruction, T data)
{
    PUTDATA_BLOCK putdata_block{};
    initIdamPutDataBlock(&putdata_block);

    putdata_block.data_type = typeIDToUDAType(typeid(T));
    putdata_block.rank = 0;
    putdata_block.count = 1;
    putdata_block.shape = nullptr;

    auto dp = new T;
    *dp = data;

    putdata_block.data = reinterpret_cast<char*>(dp);

    idamPutAPI(instruction.c_str(), &putdata_block);

    delete dp;
}

void uda::Client::put(const std::string& instruction, int8_t data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, int16_t data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, int32_t data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, int64_t data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, uint8_t data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, uint16_t data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, uint32_t data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, uint64_t data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, float data) { put_scalar(instruction, data); }
void uda::Client::put(const std::string& instruction, double data) { put_scalar(instruction, data); }

template <typename T>
void put_vector(const std::string& instruction, const std::vector<T>& data)
{
    PUTDATA_BLOCK putdata_block{};
    initIdamPutDataBlock(&putdata_block);

    putdata_block.data_type = typeIDToUDAType(typeid(T));
    putdata_block.rank = 1;
    putdata_block.count = 1;

    auto shape = new int[1];
    shape[0] = static_cast<int>(data.size());

    putdata_block.shape = shape;

    putdata_block.data = reinterpret_cast<char*>(const_cast<T*>(data.data()));

    idamPutAPI(instruction.c_str(), &putdata_block);

    delete[] shape;
}

void uda::Client::put(const std::string& instruction, const std::vector<int8_t>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<int16_t>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<int32_t>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<int64_t>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<uint8_t>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<uint16_t>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<uint32_t>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<uint64_t>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<float>& data) { put_vector(instruction, data); }
void uda::Client::put(const std::string& instruction, const std::vector<double>& data) { put_vector(instruction, data); }

uda::Client::~Client()
{
    for (auto& data : data_) {
        delete(data);
    }
    //idamFreeAll();
}
