#pragma once

#include "structs.hpp"
#include "vector3.hpp"

/*! \def NO_TEXTURE_COORDS
\brief Počet texturovacích souřadnic.
*/
#define NO_TEXTURE_COORDS 1

//class Surface;

/*! \struct Vertex
\brief Struktura popisující všechny atributy vertexu.

\author Tomáš Fabián
\version 1.0
\date 2013
*/
struct /*ALIGN*/ Vertex
{
    Vector3 position; /*!< Pozice vertexu. */
    Vector3 normal; /*!< Normála vertexu. */
    Vector3 color; /*!< RGB barva vertexu <0, 1>^3. */
    Coord2f texture_coords[NO_TEXTURE_COORDS]; /*!< Texturovací souřadnice. */
    Vector3 tangent; /*!< První osa souřadného systému tangenta-bitangenta-normála. */

    //char pad[8]; // doplnění na 64 bytů, mělo by to mít alespoň 4 byty, aby se sem vešel 32-bitový ukazatel

    //! Výchozí konstruktor.
    /*!
    Inicializuje všechny složky vertexu na hodnotu nula.
    */
    Vertex()
        : position( 0, 0, 0 )
        , normal( 0, 0, 0 )
        , color( 0, 0, 0 )
        , texture_coords{ { 0, 0 } }
        , tangent( 0, 0, 0 ) { }

    //! Obecný konstruktor.
    /*!
    Inicializuje vertex podle zadaných hodnot parametrů.

    \param position pozice vertexu.
    \param normal normála vertexu.
    \param color barva vertexu.
    \param texture_coords nepovinný ukazatel na pole texturovacích souřadnic.
    */
    Vertex( const Vector3 position, const Vector3 normal, Vector3 color, Coord2f * texture_coords = nullptr );

    //void Print();
};
