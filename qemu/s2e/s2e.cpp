#include "s2e.h"

#include <stdlib.h>

S2E *S2E::s_S2E = NULL;

extern "C"
{
int S2EInit(const char *CfgFile)
{
  S2E::GetInstance(CfgFile);
  return 0;
}

int S2EOnTbEnter(void *CpuState, int Translation)
{
  S2E *I = S2E::GetInstance();
  if (!I) {
    return 0;
  }

  return I->GetOS()->OnTbEnter(CpuState, Translation);
}

int S2EOnTbExit(void *CpuState, int Translation)
{
  S2E *I = S2E::GetInstance();
  if (!I) {
    return 0;
  }

  return I->GetOS()->OnTbExit(CpuState, Translation);
}

}

S2E* S2E::GetInstance(const char *CfgFile)
{
  if (!s_S2E) {
    s_S2E = new S2E(CfgFile);
  }
  return s_S2E;
}

S2E* S2E::GetInstance()
{
  return s_S2E;
}

S2E::S2E(const char *CfgFile)
{
  m_CfgMgr = new CConfigurationManager(CfgFile);
  m_Os = new COperatingSystem(m_CfgMgr);
  if (!m_Os->IsLoaded()) {
    delete m_Os;
    delete m_CfgMgr;
    exit(-1);
  }

  m_Os->LoadModuleInterceptors();
}

COperatingSystem *S2E::GetOS() const
{
  return m_Os;
}