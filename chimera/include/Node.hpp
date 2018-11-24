#ifndef __CHIMERA_NODE__HPP
#define __CHIMERA_NODE__HPP

#include "Entity.hpp"
#include <list>
#include <vector>

namespace Chimera {

/**
 * Class Node
 *  @author <a href="mailto:edupagotto@gmail.com">Eduardo Pagotto</a>
 *  @since 20130925
 *  @version %I%, %G%
 */
class Node : public Entity {
  public:
    /**
     * Constructor inicializa _tipo e _name
     * @param _parent ponteiro do Node Pai ou <code>nullptr</code>
     * @param _name nome do Node
     */
    Node(Node* _parent, EntityKind _type, std::string _name);

    /**
     * Destructor
     */
    virtual ~Node();

    /**
     * Entry point de visitor
     * @param v ponteiro do visitor
     */
    virtual void accept(class NodeVisitor* v) = 0;

    /**
     * Adiciona um child ao Node atual
     * @param _child Node a ser adicionado a este como filho
     */
    void addChild(Node* _child);

    /**
     * Define novo Pai, removendo o original se existir
     * @param _node Novo node pai
     */
    void setParent(Node* _node);

    /**
     * Retorna um vector com os filhos deste Node
     * @return Lista de Nodes Filhos
     */
    std::vector<Node*>* getChilds();

    /**
     * Remove Node Child deste node Pai
     * @param _child node a ser removido dos filhos
     */
    void removeChild(Node* _child);

    /**
     * Retorna o Numero de filhos deste pai ou de todos os descendentes
     * @param _recursiveCount se <code>TRUE</code> define toda a linhagem<p> se Pai
     * apoenas os locais
     * @return numero de filhos
     */
    const size_t countChilds(const bool& _recursiveCount) const;

    /**
     * Localiza child com o nome
     * @param _searchName nome a se perquisar
     * @return Node ou <code>nullptr</code> se nao encontrado
     */
    Node* findChild(const std::string& _searchName, const bool& _findInChild);

    /**
     * Localiza child com o nome
     * @param _type tipo do Node a se perquisar
     * @param _index indice do Node de mesmo tipo
     * @return Node ou <code>nullptr</code> se nao encontrado
     */
    Node* findChild(const EntityKind& _type, const int& _index, const bool& _findInChild);

    /**
     * Retorna node Pai
     * @return Node Pai ou <code>nullptr</code>
     */
    inline Node* getParent() { return parent; }

    /**
     * Define o Status de Uso do Node
     * @param _col se <code>0</code> livre <p> Se <code>1</code> Ocupado
     */
    inline void setColor(const int& _col) { color = _col; }

    /**
     * Retorna o status de uso do Node
     * @return <code>0</code> livre <p> Se <code>1</code> Ocupado
     */
    inline int getColor() const { return color; }

    /**
     * Busca sequencial em todos dos Nodes criados
     * @param _type tipo do Node
     * @param _index indice do Tipo
     * @return ponteiro do Node encontrado ou <code>nullptr</code>
     */
    // static Node *findNodeBySeq ( const EntityKind &_type, const int &_index );

    /**
     * Busca sequencial em todos dos Nodes criados
     * @param _type Tipo do Node
     * @param _name Nome do Node
     * @return ponteiro do Node encontrado ou <code>nullptr</code>
     */
    // static Node *findNodeBySeq ( const EntityKind &_type, const std::string &_name );

  private:
    /** Controle de Ocupado Livre*/
    int color;

    /** Node Pai */
    Node* parent;

    /** vetor de Filhos*/
    std::vector<Node*> vChild;

    /** Lista Sequencial de Nodes*/
    // static std::list<Node*> listNode;
};
} // namespace Chimera
#endif
