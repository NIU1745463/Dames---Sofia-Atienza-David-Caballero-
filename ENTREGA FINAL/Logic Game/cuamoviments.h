#ifndef CUAMOVIMENTS_H  
#define CUAMOVIMENTS_H

#include <string>  

// Estructura per a cada node de la cua
struct NodeMoviment
{
    std::string moviment;  // text amb el moviment
    NodeMoviment* seguent; // punter al següent moviment

    // Constructor: crea un node amb el moviment i el següent a nullptr (res)
    NodeMoviment(const std::string& mov) : moviment(mov), seguent(nullptr) {}
};

// Classe que representa una cua de moviments
class CuaMoviments
{
public:
    CuaMoviments();   
    ~CuaMoviments();  

    void carregarMoviments(const std::string& nomFitxer);  
    void afegirMoviment(const std::string& moviment);       
    std::string treureMoviment();                           
    bool buida() const;                                     
    void mostrarMoviments() const;                          
    void guardarMoviments(const std::string& nomFitxer) const;  

private:
    NodeMoviment* primer;  
    NodeMoviment* ultim;  
};

#endif
