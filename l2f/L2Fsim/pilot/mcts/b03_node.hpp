#ifndef L2FSIM_B03_NODE_HPP_
#define L2FSIM_B03_NODE_HPP_

#include <cstdio>
#include <cstdlib>

/**
 * Node for MCTS for 'beeler_glider.hpp' model
 * @version 1.0
 * @since 1.0
 *
 * @note compatibility: 'beeler_glider.hpp'; 'beeler_glider_state.hpp'; 'beeler_glider_command.hpp'
 * @note termination criterion for a node is set in 'is_terminal' method
 */

namespace L2Fsim{

class b03_node {
public:
    /**
     * Attributes
     * @param {beeler_glider_state} s; state of the node
     * @param {beeler_glider_command} incoming_action; action leading from the node's parent to the node itself
     * @param {double} average_reward; Q-value
     * @param {unsigned int} number_of_visits;
     * @param {unsigned int} depth;
     * @param {b03_node *} parent; pointer to the parent node
     * @param {std::vector<beeler_glider_command>} expendable_actions; actions available from the node's state
     * @param {std::vector<b03_node>} children; vector containing the children of the node, initialy empty
     *
     * @warning the pointer to the parent 'parent' is obsolete if the node is root, make use of the boolean 'is_root_node'
     */
    beeler_glider_state s;
    beeler_glider_command incoming_action;
    double average_reward;
    unsigned int number_of_visits;
    unsigned int depth;
    b03_node *parent;
    std::vector<beeler_glider_command> expendable_actions;
    std::vector<b03_node> children;

    /** Constructor with given state */
    b03_node(
        beeler_glider_state _s,
        std::vector<beeler_glider_command> _expendable_actions,
        double _average_reward=0.,
        unsigned int _number_of_visits=0,
        unsigned int _depth=0) :
        s(_s),
        expendable_actions(_expendable_actions),
        average_reward(_average_reward),
        number_of_visits(_number_of_visits),
        depth(_depth)
    {}

    /** Constructor with default state */
    b03_node(
        std::vector<beeler_glider_command> _expendable_actions,
        double _average_reward=0.,
        unsigned int _number_of_visits=0,
        unsigned int _depth=0) :
        s(),
        expendable_actions(_expendable_actions),
        average_reward(_average_reward),
        number_of_visits(_number_of_visits),
        depth(_depth)
    {}

    /** Return a reference on the state attribute */
    beeler_glider_state & get_state() {return s;}

    /** Boolean test for termination criterion */
    bool is_terminal() {
        bool answer = (s.z < 0.) ? true : false;
        return answer;
    }

    /** Boolean test for a node being fully expanded or not */
    bool is_fully_expanded() {
        bool answer = (expendable_actions.size() == 0) ? true : false;
        return answer;
    }

    /**
     * Get a reference on the last child that was created i.e. end position of 'children' attribute
     * @return {b03_node}
     */
    b03_node & get_last_child() {
        return children.back();
    }
};

}

#endif