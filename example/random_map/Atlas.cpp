/*!
 * \file Atlas.cpp
 *
 * \author zjhlogo
 * \date 2020/01/23
 *
 * 
 */
#include "Atlas.h"

Atlas::Atlas()
{
}

Atlas::~Atlas()
{
}

const Atlas::Piece* Atlas::findPiece(const NS::tstring& id)
{
    const auto& it = m_pieceMap.find(id);
    if (it == m_pieceMap.end()) return nullptr;
    return it->second;
}

const Atlas::Piece* Atlas::getPiece(int index)
{
    assert(index >= 0 && index < m_numPieces);

    return &m_pieceList[index];
}
