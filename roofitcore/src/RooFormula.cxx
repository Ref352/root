/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitTools
 *    File: $$
 * Authors:
 *   WV, Wouter Verkerke, University of California Santa Barbara, verkerke@slac.stanford.edu
 * History:
 *   05-Mar-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/
#include "BaBar/BaBar.hh"

#include "RooFitCore/RooFormula.hh"
#include "TROOT.h"
#include <iostream.h>

ClassImp(RooFormula)

RooFormula::RooFormula() : TFormula()
{
}

RooFormula::RooFormula(const char* name, const char* formula, RooArgSet& list) : TFormula(), _origList(&list)
{
  SetName(name) ;
  SetTitle(formula) ;

  if (Compile()) {
    cout << "RooFormula: compile error" << endl ;
    return ;
  }
}

RooFormula::RooFormula(const RooFormula& other) : 
  TFormula(other), _origList(other._origList)
{
  int i ;
  for (i=0 ; i<other._useList.GetEntries() ; i++) {
    _useList.Add(other._useList.At(i)) ;
  }
}


RooFormula::~RooFormula() 
{
}


RooArgSet& RooFormula::actualDependents()
{
  // Return list of actual dependents
  // (formula might not use all given input parameters)
  static RooArgSet set("actualDependents") ;
  set.Clear() ;
  int i ;
  for (i=0 ; i<_useList.GetEntries() ; i++) {
    set.Add(_useList[i]) ;
  }
  return set ;
}


void RooFormula::dump() {
  // DEBUG: Dump state information
  int i ;
  cout << "RooFormula::dump()" << endl ;
  for (i=0 ; i<_useList.GetEntries() ; i++) {
    cout << "[" << i << "] = " << (void*) _useList.At(i) << " = " << ((RooAbsValue*)_useList.At(i))->GetVar() << endl ;
  }
}


Bool_t RooFormula::changeDependents(RooArgSet& newDeps, Bool_t mustReplaceAll) 
{
  //Change current servers to new servers with the same name given in list
  Bool_t errorStat(kFALSE) ;
  int i ;
  for (i=0 ; i<_useList.GetEntries() ; i++) {
    RooAbsValue* replace = (RooAbsValue*) newDeps.find(_useList[i]->GetName()) ;
    if (replace) {
      _useList[i] = replace ;
    } else if (mustReplaceAll) {
      cout << "RooFormula::changeDependents: cannot find replacement for " 
	   << _useList[i]->GetName() << endl ;
      errorStat = kTRUE ;
    }
  }  
  return errorStat ;
}


Double_t
RooFormula::DefinedValue(Int_t code) {
  // Return current value for parameter indicated by internal reference code
  return ((RooAbsValue*)_useList.At(code))->GetVar() ;
}


Int_t 
RooFormula::DefinedVariable(TString &name)
{
  // Defined internal reference code for given named variable 
  RooAbsValue* rrv= (RooAbsValue*) _origList->find(name) ;
  if (!rrv) return -1 ;
  _useList.Add(rrv) ;

  return _useList.GetEntries()-1 ;
}



