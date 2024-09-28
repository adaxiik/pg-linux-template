#pragma once
#include "vertex.hpp"

class Surface; // dopředná deklarace třídy

/*! \class Triangle
\brief A class representing single triangle in 3D.

\author Tomáš Fabián
\version 1.1
\date 2013-2018
*/
class Triangle
{
public:
    //! Výchozí konstruktor.
    /*!
    Inicializuje všechny složky trojúhelníku na hodnotu nula.
    */
    Triangle(): vertices_{ Vertex(), Vertex(), Vertex() } { }

    //! Obecný konstruktor.
    /*!
    Inicializuje trojúhelník podle zadaných hodnot parametrů.

    \param v0 první vrchol trojúhelníka.
    \param v1 druhý vrchol trojúhelníka.
    \param v2 třetí vrchol trojúhelníka.
    \param surface ukazatel na plochu, jíž je trojúhelník členem.
    */
    Triangle( const Vertex & v0, const Vertex & v1, const Vertex & v2, Surface * surface = nullptr );

    //! I-tý vrchol trojúhelníka.
    /*!
    \param i index vrcholu trojúhelníka.

    \return I-tý vrchol trojúhelníka.
    */
    Vertex vertex( const int i );

    bool is_degenerate() const;

private:
    Vertex vertices_[3]; /*!< Vrcholy trojúhelníka. Nic jiného tu nesmí být, jinak padne VBO v OpenGL! */
};
