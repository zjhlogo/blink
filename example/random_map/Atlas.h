#pragma once
#include <render/texture/Texture2D.h>
#include <unordered_map>

class Atlas
{
public:
    class Piece
    {
    public:
        const char* id;
        glm::vec2 size;
        glm::vec2 trimSize;
        glm::vec2 offset;
        glm::vec2 uvs[4];
    };

    typedef std::unordered_map<blink::tstring, const Piece*> PieceMap;

public:
    Atlas();
    virtual ~Atlas();

    virtual bool initialize() = 0;

    std::shared_ptr<blink::Texture2D> getTexture() const { return m_texture; };
    const Piece* findPiece(const blink::tstring& id);
    const Piece* getPiece(int index);

protected:
    std::shared_ptr<blink::Texture2D> m_texture{};
    PieceMap m_pieceMap;

    const Piece* m_pieceList{};
    int m_numPieces{};

};
