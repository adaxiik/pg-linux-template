#pragma once

#include <vector>
#include "vector3.hpp"
#include "surface.hpp"

/*! \fn int LoadOBJ( const char * file_name, Vector3 & default_color, std::vector<Surface *> & surfaces, std::vector<Material *> & materials )
\brief Načte geometrii z OBJ souboru \a file_name.
\note Při exportu z 3ds max je nutné nastavit systémové jednotky na metry:
Customize -> Units Setup Metric (Meters)
System Unit Setup -> 1 Unit = 1,0 m a zaškrtnout Respect System Units in File
\see
\param file_name úplná cesta k OBJ souboru včetně přípony.
\param surfaces pole ploch, do kterého se budou ukládat načtené plochy.
\param materials pole materiálů, do kterého se budou ukládat načtené materiály.
\param flip_yz rotace kolem osy x o + 90st.
\param default_color výchozí barva vertexu.
*/
int LoadOBJ( const char * file_name, std::vector<Surface *> & surfaces, std::vector<Material *> & materials,
    const bool flip_yz = false, const Vector3 default_color = Vector3( 0.5f, 0.5f, 0.5f ) );
