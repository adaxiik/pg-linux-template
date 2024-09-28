#pragma once

#include "vector3.hpp"

/*! \class Matrix3x3
\brief Reálná matice 3x3 uspořádaná po řádcích.

\author Tomáš Fabián
\version 1.1
\date 2018
*/
class Matrix3x3
{
public:
    //! Výchozí konstruktor.
    /*!
    Inicializace na matici identity.
    */
    Matrix3x3();

    //! Výchozí konstruktor.
    /*!
    Inicializace matice zadanými hodnotami. První index označuje řádek, druhý index patří sloupci.
    */
    Matrix3x3( const float m00, const float m01, const float m02,
        const float m10, const float m11, const float m12,
        const float m20, const float m21, const float m22 );

    Matrix3x3( const Vector3 basis_x, const Vector3 basis_y, const Vector3 basis_z );

    //! Transpozice matice.
    /*!
    Provede traspozici matice vzájemnou výměnou řádků a sloupců.
    */
    Matrix3x3 Transpose() const;

    //! Nastaví zadaný prvek matice na novou hodnotu.
    /*!
    \param row řádek matice.
    \param column sloupec matice.
    \param value nová hodnota prvku matice.
    */
    void set( const int row, const int column, const float value );

    //! Vrátí zadaný prvek matice.
    /*!
    \param row řádek matice.
    \param column sloupec matice.
    \return Požadovaný prvek matice.
    */
    float get( const int row, const int column ) const;

    friend Vector3 operator*( const Matrix3x3 & a, const Vector3 & b );
    friend Matrix3x3 operator*( const Matrix3x3 & a, const Matrix3x3 & b );

private:
    union
    {
        /* Row-major
        m00 m01 m02
        m10 m11 m12
        m20 m21 m22
        */

        struct
        {
            float m00_;
            float m01_;
            float m02_;

            float m10_;
            float m11_;
            float m12_;

            float m20_;
            float m21_;
            float m22_;
        };

        float data_[3 * 3];
    };
};

typedef Matrix3x3 Matrix3;
