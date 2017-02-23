/*!
 * \file ModulesIOList.h
 * \brief For generate modules IO list and parameters list.
 *
 *
 *
 * \author Junzhi Liu, Liangjun Zhu
 * \version 1.0
 * \date July 2015
 *
 *
 */
#pragma once

#include <map>
#include <string>
#include <vector>
#include "ParamInfo.h"
#include "SimulationModule.h"
#include "tinyxml.h"
#include "SEIMS_ModuleSetting.h"
#include "MetadataInfoConst.h"

#ifdef windows
#include <Windows.h>
#endif

using namespace std;

/*!
 * \ingroup modulesiolist
 * \class ModulesIOList
 *
 * \brief
 *
 *
 *
 */
string _GetApplicationPath();

/// Write text to a file.
void writeText(string &filename, vector <vector<string>> &data);

void writeText(string &filename, vector <string> &data);

void writeText(string &filename, map <string, vector<string>> &data);

string DimentionType2String(dimensionTypes dimtyp);

class ModulesIOList {
public:
    ModulesIOList(const string &ModuleIDsFile, const string &dllPath);

    ~ModulesIOList(void);

    vector <vector<string>> moduleInfoList;
    map <string, vector<string>>
        moduleParamsInfoList; ///< map<paramId, veector<unit,description,dimentionType,inputModuleIDs,outputModuleIDs> >
    map <string, vector<string>> paramsInfoList;

    void FindOutputParameter(string &outputID, int &iModule, ParamInfo *&paraInfo);

    string GetModuleID(int i) { return m_moduleIDs[i]; }

    vector <string> GetModuleIDs() { return m_moduleIDs; }

private:
    typedef SimulationModule *(*InstanceFunction)(void);

    typedef const char *(*MetadataFunction)(void);

    /// function pointers used to create class instance
    map <string, InstanceFunction> m_instanceFuncs;
    map <string, MetadataFunction> m_metadataFuncs;

    string m_dllPath;
    vector <string> m_moduleIDs;
#ifdef windows
    vector<HINSTANCE> m_dllHandles;
#else
    vector<void *> m_dllHandles;
#endif
    map<string, SEIMSModuleSetting *> m_settings;
    map<string, const char *> m_metadata;
    map <string, vector<ParamInfo>> m_parameters;
    map <string, vector<ParamInfo>> m_inputs;
    map <string, vector<ParamInfo>> m_outputs;

private:
    /// read the config.fig file and initialize
    void Init(const string &configFileName);

    bool LoadModuleInfoFromFile(const char *filename, vector <vector<string>> &settings);

    void ReadModulesListFile(const char *configFileName);

    void ReadDLL(string &moduleID, string &dllID);

    SimulationModule *GetInstance(string &moduleID);

    dimensionTypes MatchType(string strType);

    bool IsConstantInputFromName(string &name);

    void ReadParameterSetting(string &moduleID, TiXmlDocument &doc, SEIMSModuleSetting *setting);

    void ReadInputSetting(string &moduleID, TiXmlDocument &doc, SEIMSModuleSetting *setting);

    void ReadOutputSetting(string &moduleID, TiXmlDocument &doc, SEIMSModuleSetting *setting);

    string GetComparableName(string &paraName);

    ParamInfo *FindDependentParam(ParamInfo &paramInfo);

    void GenerateModulesInfoList();

    void GenerateParamsInfoList();  ///< parameters from database
    void GenerateModuleParamsInfoList(); ///< parameters generated by modules
    string GetInvokeModulesIDs(ParamInfo &paramIns);

    string GetOutputModulesIDs(ParamInfo &paramIns);

    string GetDBParamsInvokeModulesIDs(ParamInfo &paramIns);
};
