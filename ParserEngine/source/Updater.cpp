/*
 * Updater.cpp
 *
 *  Created on: 02.06.2014
 *      Author: dmitry
 */

#include "Updater.h"

cUpdater::cUpdater()
{
	mpCurrentUpdates = 0;

	m_mapUpdateContainer.clear();
}
cUpdater::~cUpdater()
{

}

void cUpdater::OnDraw()
{
	if(mpCurrentUpdates)
	{
		for(tUpdatableListIt Iter = mpCurrentUpdates->begin(); Iter!=mpCurrentUpdates->end(); ++Iter)
		{
			(*Iter)->OnDraw();
		}
	}
}
void cUpdater::OnStart()
{
	tUpdateContainerMapIt ContIter = m_mapUpdateContainer.begin();
	while(ContIter != m_mapUpdateContainer.end())
	{
		tUpdatableListIt UpIter = ContIter->second.begin();
		while(UpIter != ContIter->second.end())
		{
			(*UpIter)->OnStart();
			++UpIter;
		}
		++ContIter;
	}
}
void cUpdater::Reset()
{
	tUpdateContainerMapIt ContIter = m_mapUpdateContainer.begin();
	while(ContIter != m_mapUpdateContainer.end())
	{
		tUpdatableList *pUpdates = &ContIter->second;
		tUpdatableListIt UpIter = pUpdates->begin();
		while(UpIter != pUpdates->end())
		{
			iUpdatable *pUpdate = *UpIter;
			pUpdate->Reset();
			++UpIter;
		}
		++ContIter;
	}
}
void cUpdater::OnExit()
{
	tUpdateContainerMapIt ContIter = m_mapUpdateContainer.begin();
	while(ContIter != m_mapUpdateContainer.end())
	{
		tUpdatableListIt UpIter = ContIter->second.begin();
		while(UpIter != ContIter->second.end())
		{
			(*UpIter)->OnExit();
			++UpIter;
		}
		++ContIter;
	}
}
void cUpdater::Update()
{
	if(mpCurrentUpdates)
	{
		for(tUpdatableListIt Iter = mpCurrentUpdates->begin(); Iter!=mpCurrentUpdates->end(); ++Iter)
		{
			(*Iter)->Update();
		}
	}
}

void cUpdater::PostUpdate()
{
	if(mpCurrentUpdates)
	{
		for(tUpdatableListIt Iter = mpCurrentUpdates->begin(); Iter!=mpCurrentUpdates->end(); ++Iter)
		{
			(*Iter)->PostUpdate();
		}
	}
}

bool cUpdater::SetContainer(std::string asContainer)
{
	tUpdateContainerMapIt It = m_mapUpdateContainer.find(asContainer);
	if(It == m_mapUpdateContainer.end())
		return false;

	msCurrentUpdates = asContainer;
	mpCurrentUpdates = &It->second;

	return true;
}

std::string cUpdater::GetCurrentContainerName()
{
	if(!mpCurrentUpdates)
		return "";
	return msCurrentUpdates;
}

bool cUpdater::AddContainer(std::string asName)
{
	tUpdateContainerMap::value_type val = tUpdateContainerMap::value_type(asName, tUpdatableList());
	m_mapUpdateContainer.insert(val);
	return true;
}

bool cUpdater::AddUpdate(std::string asContainer, iUpdatable *apUpdate)
{
	if(!apUpdate)
		return false;

	tUpdateContainerMapIt it = m_mapUpdateContainer.find(asContainer);
	if(it == m_mapUpdateContainer.end()) return false;

	it->second.push_back(apUpdate);

	return true;
}


