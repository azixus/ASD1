/*
 * Buffer circulaire (2)

Complétez la classe générique ArrayDeque pour qu'elle passe ce codecheck.

Cette classe met en oeuvre le comportement de double-ended queue en utilisant un buffer circulaire de capacité variable. Les données sont stockées dans un tableau buffer alloué dynamiquement. L'allocation et la construction sont séparées, de sorte que la capacité non utilisée n'est pas construite. Destruction et libération de la mémoire le sont également.

Vous ne pouvez ajouter aucun attribut privé à la classe ni modifier les attributs et le constructeur fourni. Vous ne pouvez ajouter comme méthodes publiques que celles nécessaires pour passer les tests d'API. Vous pouvez ajouter toute méthode privée dont vous auriez besoin

Toutes les méthodes et constructeurs doivent offrir des garanties fortes, hormis pour la capacité qui a le droit de changer.

Pour ce laboratoire, il n'est pas nécessaire de générer des exceptions. En cas de mauvaise utilisation, le comportement est non spécifié.
 */
#ifndef ArrayDeque_h
#define ArrayDeque_h

#include <algorithm>
#include <stdexcept>

template < class T >
class ArrayDeque
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using rvalue_reference = T&&;
    using pointer = T*;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    pointer buffer;
    size_type capacite;
    size_type debut;
    size_type taille;

    /**
    * Calcul l'adresse physique d'une variable dans l'ArrayDeque grâce à une adresse logique
    * @param i_logique indique l'index logigue de la variable
    * @return un size_t donnant l'index physique de la variable
    */
    size_type index_physique(size_type i_logique) const {
        if (debut + i_logique == 0)
            return 0;
        //calcul de l'index physique de la donnée dans l'ArrayDeque
        size_type ip = (debut + i_logique) % capacity();
        if (ip >= 0)
            return ip;
        else
            return ip + capacity();
    }

    /**
     * Permet de doubler la capacité de l'ArrayDeque
     */
    void extend() {
        size_type newCapacity = capacity() == 0 ? 1 : capacity() * 2;

        ArrayDeque tmp(newCapacity);
        //pointer tmp = (pointer) ::operator new(newCapacity * sizeof(value_type));

        for (size_type i = 0; i < taille; ++i) {
            new(tmp.buffer + i) value_type(std::move(buffer[index_physique(i)]));
            ++tmp.taille;
        }

        std::swap(tmp.buffer, buffer);
        std::swap(tmp.capacite, capacite);
        std::swap(tmp.debut, debut);
    }

public:
    ArrayDeque(size_type cap = 0) : debut(0), taille(0), capacite(cap)
    {
        buffer = capacite != 0 ?
                 (pointer) ::operator new(capacite * sizeof(value_type))
                               : nullptr;
    }

    ArrayDeque(const ArrayDeque& rhs) : ArrayDeque(rhs.capacite) {
        debut = rhs.debut;
        for (size_type i = 0; i < rhs.taille; ++i) {
            new(buffer + index_physique(i)) value_type(rhs.at(i));
            ++taille;
        }
    }

    ArrayDeque& operator=(const ArrayDeque& rhs) {
        //Appel du constructeur par copie
        ArrayDeque tmp = rhs;

        std::swap(buffer, tmp.buffer);

        return *this;
    }

    ~ArrayDeque() {
        for (size_type i = 0; i < taille; ++i) {
            buffer[index_physique(i)].~value_type();
        }

        ::operator delete(buffer);
    }

    /**
     * Retourne la taille de l'ArrayDeque
     * @return size_t donnant la taille de l'ArrayDeque
     */
    size_type size() const {
        return taille;
    }

    /**
     * Retourne la capacité de l'ArrayDeque
     * @return size_t indiquant la capacité de l'ArrayDeque
     */
    size_type capacity() const {
        return capacite;
    }

    /**
     * Permet de vérifier si l'ArrayDeque est vide
     * @return vrai si vide, faux sinon
     */
    bool empty() const {
        return taille == 0;
    }

    /**
     * Permet de retourner le dernier élément de l'ArrayDeque
     * @return référence de type T sur le dernier élément
     */
    reference back() {
        //ArrayDeque vide, comportement indéterminé
        if (this->empty())
            throw;

        return buffer[index_physique(taille - 1)];
    }

    /**
     * Fonction back surchargée pour pouvoir l'utiliser avec une ArrayDeque constante
     * @return référence constante de type T du dernier élément
     */
    const_reference back() const {
        //ArrayDeque vide, comportement indéterminé
        if (this->empty()) {
            throw;
        }

        return buffer[index_physique(taille - 1)];
    }

    reference at(size_type n) {
        //ArrayDeque vide, comportement indéterminé
        if (this->empty())
            throw;

        return buffer[index_physique(n)];
    }

    const_reference at(size_type n) const {
        //ArrayDeque vide, comportement indéterminé
        if (this->empty()) {
            throw;
        }

        return buffer[index_physique(n)];
    }

    /**
     * Renvoie la première valeur de l'ArrayDeque
     * @return référence de type T de la première valeur
     */
    reference front() {
        //ArrayDeque vide, comportement indéterminé
        if (this->empty())
            throw;

        return buffer[debut];
    }

    /**
     * Surcharge pour pouvoir renvoyer la première valeur pour une ArrayDeque constante
     * @return référence constante de la première valeur
     */
    const_reference front() const {
        //ArrayDeque vide, comportement indéterminé
        if (this->empty())
            throw;

        return buffer[debut];
    }

    /**
     * Ajoute une valeur en fin de l'ArrayDeque
     * @param val valeur a ajouter a l'ArrayDeque
     */
    void push_back(const_reference val) {
        //Vérification que la taille n'est pas déjà plus grande ou égal à la capacité
        //dans ce cas on l'augmente, sinon nous aurons un dépassement
        if (taille >= this->capacity())
            extend();

        new(buffer + index_physique(taille)) value_type(val);
        ++taille;
    }

    void push_back(rvalue_reference val) {
        //Vérification que la taille n'est pas déjà plus grande ou égal à la capacité
        //dans ce cas on l'augmente, sinon nous aurons un dépassement
        if (taille >= this->capacity())
            extend();

        new(buffer + index_physique(taille)) value_type(std::move(val));
        ++taille;
    }

    /**
     * Supprime la dernière valeur de l'ArrayDeque
     */
    void pop_back() {
        //Si l'ArrayDeque est vide impossible de supprimer
        if (this->empty())
            throw;

        buffer[index_physique(taille - 1)].~value_type();
        --taille;
    }

    /**
    * Permet d'ajouter une valeur en début de l'ArrayDeque
    * @param val valeur a ajouter a l'ArrayDeque
    */
    void push_front(const_reference val) {
        //Vérification que la taille n'est pas déjà plus grande ou égal à la capacité
        //dans ce cas on l'augmente, sinon nous aurons un dépassement
        if (taille >= capacity())
            extend();

        //Ameliorer ?
        new(buffer + index_physique(capacity() - 1)) value_type(val);
        debut = index_physique(capacity() - 1);
        ++taille;
    }

    /**
    * Permet d'ajouter une valeur en début de l'ArrayDeque
    * @param val valeur a ajouter a l'ArrayDeque
    */
    void push_front(rvalue_reference val) {
        //Vérification que la taille n'est pas déjà plus grande ou égal à la capacité
        //dans ce cas on l'augmente, sinon nous aurons un dépassement
        if (taille >= capacity())
            extend();

        new(buffer + index_physique(capacity() - 1)) value_type(std::move(val));
        debut = index_physique(capacity() - 1);
        ++taille;
    }

    /**
     * Supprime la première valeur de l'ArrayDeque
     */
    void pop_front() {
        //impossible de supprimer
        if (this->empty())
            throw;

        buffer[debut].~value_type();
        debut = index_physique(1);
        --taille;
    }

};

#endif /* ArrayDeque_hpp */