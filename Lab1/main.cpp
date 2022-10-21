/*------------------------------------------------------------------------------
 *
 *      File includes
 *
 *------------------------------------------------------------------------------*/
#include <stdint.h>

#include <iostream>
using std::cout;
using std::cin;

#include "template.h"

/*------------------------------------------------------------------------------
 *
 *      Typedefs and constants
 *
 *------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 *      Global vars
 *
 *------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 *      File scope vars
 *
 *------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 *      Functions and Methods
 *
 *------------------------------------------------------------------------------*/
/**
 * Main function.
 *
 * @param[in] argc - not used, declared for compatibility
 * @param[in] argv - not used, declared for compatibility
 * @returns int    - not used, declared for compatibility
 */
int main(int argc, char ** argv)
{
    cout << "Lab 01 de sistemas embarcados\n";
    cout << "Valores de simbolos pre-definidos:\n";
    cout << "__cplusplus:";
    cout << __cplusplus;
    cout << "\n__DATE__:";
    cout << __DATE__; 
    cout << "\n__FILE__:";
    cout << __FILE__; 
    
    float num1, num2, resultado;
    num2 = 13.9;
    cout << "\nDigite um valor (float) que deseja somar:";
    cin >> num1;
    cout << num1;
    resultado = num1 + num2;
    cout << "\nResultado: " << num1 << " + " << num2 << " = " << resultado; 
 
    return 0;
}
