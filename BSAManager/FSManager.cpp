/***** BEGIN LICENSE BLOCK *****

BSD License

Copyright (c) 2005-2012, NIF File Format Library and Tools
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the NIF File Format Library and Tools project may not be
   used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***** END LICENCE BLOCK *****/


#include "FSManager.h"
#include "FSEngine.h"
#include "FSBSA.h"
#include <algorithm>


//! Global BSA file manager
static FSManager *theFSManager = nullptr;

FSManager* FSManager::get()
{
	if (!theFSManager)
		theFSManager = new FSManager();
	return theFSManager;
}

std::list<FSArchiveFile*> FSManager::archiveList()
{
	std::list<FSArchiveFile*> archives;

	std::transform(get()->archives.begin(), get()->archives.end(), back_inserter(archives),
		[](std::map<std::string, FSArchiveHandler*>::value_type& val){ return val.second->getArchive(); });

	return archives;
}

FSManager::FSManager()
{
	wxArrayString list;
	list = autodetectArchives();

	for (auto an : list)
	{
		if (FSArchiveHandler *a = FSArchiveHandler::openArchive(an))
			archives[an.ToStdString()] = a;
	}
}

FSManager::~FSManager()
{
	for (auto it : archives)
		delete it.second;

	archives.clear();
}

wxArrayString FSManager::autodetectArchives()
{
	wxArrayString list;

	//TODO: GET GAME DATA PATH FROM REGISTRY
	//if (Config["GameDataPath"].empty())
	//	return list;

	//wxString path = Config["GameDataPath"];

	wxString path = wxGetCwd();
	if (!path.IsEmpty())
	{
		wxArrayString files;
		wxDir::GetAllFiles(path, &files, "*.bsa", wxDIR_FILES);

		for (auto file : files)
			list.Add(file);
	}

	return list;
}
