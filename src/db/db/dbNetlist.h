
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2019 Matthias Koefferlein

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef _HDR_dbNetlist
#define _HDR_dbNetlist

#include "dbCommon.h"
#include "dbCircuit.h"
#include "dbDeviceClass.h"
#include "dbDeviceModel.h"

#include "tlVector.h"

#include <string>

namespace db
{

/**
 *  @brief The netlist class
 *
 *  This class represents a hierarchical netlist.
 *  The main components of a netlist are circuits and device classes.
 *  The circuits represent cells, the device classes type of devices.
 */
class DB_PUBLIC Netlist
  : public gsi::ObjectBase, public tl::Object
{
public:
  typedef tl::shared_collection<Circuit> circuit_list;
  typedef circuit_list::const_iterator const_circuit_iterator;
  typedef circuit_list::iterator circuit_iterator;
  typedef tl::shared_collection<DeviceClass> device_class_list;
  typedef device_class_list::const_iterator const_device_class_iterator;
  typedef device_class_list::iterator device_class_iterator;
  typedef tl::shared_collection<DeviceModel> device_model_list;
  typedef device_model_list::const_iterator const_device_model_iterator;
  typedef device_model_list::iterator device_model_iterator;
  typedef tl::vector<Circuit *>::const_iterator top_down_circuit_iterator;
  typedef tl::vector<const Circuit *>::const_iterator const_top_down_circuit_iterator;
  typedef tl::vector<Circuit *>::const_reverse_iterator bottom_up_circuit_iterator;
  typedef tl::vector<const Circuit *>::const_reverse_iterator const_bottom_up_circuit_iterator;

  /**
   *  @brief Constructor
   *
   *  This constructor creates an empty hierarchical netlist
   */
  Netlist ();

  /**
   *  @brief Copy constructor
   */
  Netlist (const Netlist &other);

  /**
   *  @brief Destructor
   */
  ~Netlist ();

  /**
   *  @brief Assignment
   */
  Netlist &operator= (const Netlist &other);

  /**
   *  @brief Clears the netlist
   */
  void clear ();

  /**
   *  @brief Returns a string representation of the netlist
   *
   *  This method is basically intended to testing.
   */
  std::string to_string () const;

  /**
   *  @brief Starts a sequence of operations during which topology updates are not desired
   *
   *  If the hierarchy is modified, the topology information (top-down order, children
   *  and parent information) may be recomputed frequently. This may cause performance issues
   *  and may not be desired.
   *
   *  Calling this method will bring the netlist into a state in which updates on the
   *  list will not happen. Using "unlock" will end this state.
   *
   *  "lock" and "unlock" are incremental and can be nested. Use "NetlistLocker" for safe locking
   *  and unlocking.
   *
   *  Before lock, the state will be validated, so inside the locked operation, the topology
   *  information will be valid with respect to the initial state.
   */
  void lock ();

  /**
   *  @brief Ends a sequence of operations during which topology updates are not desired
   *  See "lock" for more details.
   */
  void unlock ();

  /**
   *  @brief Adds a circuit to this netlist
   *
   *  The netlist takes over ownership of the object.
   */
  void add_circuit (Circuit *circuit);

  /**
   *  @brief Deletes a circuit from the netlist
   */
  void remove_circuit (Circuit *circuit);

  /**
   *  @brief Begin iterator for the circuits of the netlist (non-const version)
   */
  circuit_iterator begin_circuits ()
  {
    return m_circuits.begin ();
  }

  /**
   *  @brief End iterator for the circuits of the netlist (non-const version)
   */
  circuit_iterator end_circuits ()
  {
    return m_circuits.end ();
  }

  /**
   *  @brief Begin iterator for the circuits of the netlist (const version)
   */
  const_circuit_iterator begin_circuits () const
  {
    return m_circuits.begin ();
  }

  /**
   *  @brief End iterator for the circuits of the netlist (const version)
   */
  const_circuit_iterator end_circuits () const
  {
    return m_circuits.end ();
  }

  /**
   *  @brief Gets the circuit with the given name
   *
   *  If no circuit with that name exists, null is returned.
   */
  Circuit *circuit_by_name (const std::string &name)
  {
    return m_circuit_by_name.object_by (name);
  }

  /**
   *  @brief Gets the circuit with the given name (const version)
   *
   *  If no circuit with that name exists, null is returned.
   */
  const Circuit *circuit_by_name (const std::string &name) const
  {
    return m_circuit_by_name.object_by (name);
  }

  /**
   *  @brief Gets the circuit with the given cell index
   *
   *  If no circuit with that cell index exists, null is returned.
   */
  Circuit *circuit_by_cell_index (db::cell_index_type cell_index)
  {
    return m_circuit_by_cell_index.object_by (cell_index);
  }

  /**
   *  @brief Gets the circuit with the given cell index (const version)
   *
   *  If no circuit with that cell index exists, null is returned.
   */
  const Circuit *circuit_by_cell_index (db::cell_index_type cell_index) const
  {
    return m_circuit_by_cell_index.object_by (cell_index);
  }

  /**
   *  @brief Gets the top-down circuits iterator (begin)
   *  This iterator will deliver the circuits in a top-down way - i.e. child circuits
   *  will always come after parent circuits.
   *  The first "top_circuit_count" elements will be top circuits (those which are not
   *  referenced by other circuits).
   */
  top_down_circuit_iterator begin_top_down ();

  /**
   *  @brief Gets the top-down circuits iterator (end)
   */
  top_down_circuit_iterator end_top_down ();

  /**
   *  @brief Gets the top-down circuits iterator (begin, const version)
   *  This iterator will deliver the circuits in a top-down way - i.e. child circuits
   *  will always come after parent circuits.
   *  The first "top_circuit_count" elements will be top circuits (those which are not
   *  referenced by other circuits).
   */
  const_top_down_circuit_iterator begin_top_down () const;

  /**
   *  @brief Gets the top-down circuits iterator (end, const version)
   */
  const_top_down_circuit_iterator end_top_down () const;

  /**
   *  @brief Gets the number of top circuits
   *  Top circuits are those which are not referenced by other circuits.
   *  In a well-formed netlist there is a single top-level circuit.
   */
  size_t top_circuit_count () const;

  /**
   *  @brief Gets the bottom-up circuits iterator (begin)
   *  This iterator will deliver the circuits in a bottom-up way - i.e. child circuits
   *  will always come before parent circuits.
   */
  bottom_up_circuit_iterator begin_bottom_up ();

  /**
   *  @brief Gets the bottom-up circuits iterator (end)
   */
  bottom_up_circuit_iterator end_bottom_up ();

  /**
   *  @brief Gets the bottom-up circuits iterator (begin, const version)
   *  This iterator will deliver the circuits in a bottom-up way - i.e. child circuits
   *  will always come before parent circuits.
   */
  const_bottom_up_circuit_iterator begin_bottom_up () const;

  /**
   *  @brief Gets the bottom-up circuits iterator (end, const version)
   */
  const_bottom_up_circuit_iterator end_bottom_up () const;

  /**
   *  @brief Adds a device class to this netlist
   *
   *  The netlist takes over ownership of the object.
   */
  void add_device_class (DeviceClass *device_class);

  /**
   *  @brief Deletes a device class from the netlist
   */
  void remove_device_class (DeviceClass *device_class);

  /**
   *  @brief Begin iterator for the device classes of the netlist (non-const version)
   */
  device_class_iterator begin_device_classes ()
  {
    return m_device_classes.begin ();
  }

  /**
   *  @brief End iterator for the device classes of the netlist (non-const version)
   */
  device_class_iterator end_device_classes ()
  {
    return m_device_classes.end ();
  }

  /**
   *  @brief Begin iterator for the device classes of the netlist (const version)
   */
  const_device_class_iterator begin_device_classes () const
  {
    return m_device_classes.begin ();
  }

  /**
   *  @brief End iterator for the device classes of the netlist (const version)
   */
  const_device_class_iterator end_device_classes () const
  {
    return m_device_classes.end ();
  }

  /**
   *  @brief Adds a device model to this netlist
   *
   *  The netlist takes over ownership of the object.
   */
  void add_device_model (DeviceModel *device_model);

  /**
   *  @brief Deletes a device model from the netlist
   */
  void remove_device_model (DeviceModel *device_model);

  /**
   *  @brief Begin iterator for the device models of the netlist (non-const version)
   */
  device_model_iterator begin_device_models ()
  {
    return m_device_models.begin ();
  }

  /**
   *  @brief End iterator for the device models of the netlist (non-const version)
   */
  device_model_iterator end_device_models ()
  {
    return m_device_models.end ();
  }

  /**
   *  @brief Begin iterator for the device models of the netlist (const version)
   */
  const_device_model_iterator begin_device_models () const
  {
    return m_device_models.begin ();
  }

  /**
   *  @brief End iterator for the device models of the netlist (const version)
   */
  const_device_model_iterator end_device_models () const
  {
    return m_device_models.end ();
  }

  /**
   *  @brief Purge unused nets
   *
   *  This method will purge all nets which return "floating".
   */
  void purge_nets ();

  /**
   *  @brief Creates pins for top-level circuits
   *
   *  This method will turn all named nets of top-level circuits (such that are not
   *  referenced by subcircuits) into pins. This method can be used before purge to
   *  avoid that purge will remove nets which are directly connecting to subcircuits.
   */
  void make_top_level_pins ();

  /**
   *  @brief Purge unused nets, circuits and subcircuits
   *
   *  This method will purge all nets which return "floating". Circuits which don't have any
   *  nets (or only floating ones) and removed. Their subcircuits are disconnected.
   */
  void purge ();

  /**
   *  @brief Combine devices
   *
   *  This method will combine devices that can be combined according
   *  to their device classes "combine_devices" method.
   */
  void combine_devices ();

private:
  friend class Circuit;

  circuit_list m_circuits;
  device_class_list m_device_classes;
  device_model_list m_device_models;
  bool m_valid_topology;
  int m_lock_count;
  tl::vector<Circuit *> m_top_down_circuits;
  tl::vector<tl::vector<Circuit *> > m_child_circuits;
  tl::vector<tl::vector<Circuit *> > m_parent_circuits;
  size_t m_top_circuits;
  object_by_attr<Netlist, Netlist::circuit_iterator, name_attribute<Circuit> > m_circuit_by_name;
  object_by_attr<Netlist, Netlist::circuit_iterator, cell_index_attribute<Circuit> > m_circuit_by_cell_index;

  void invalidate_topology ();
  void validate_topology ();
  void circuits_changed ();

  const tl::vector<Circuit *> &child_circuits (Circuit *circuit);
  const tl::vector<Circuit *> &parent_circuits (Circuit *circuit);
};

/**
 *  @brief A helper class using RAII for safe locking/unlocking
 */
class DB_PUBLIC NetlistLocker
{
public:
  NetlistLocker (Netlist *netlist)
    : mp_netlist (netlist)
  {
    if (mp_netlist.get ()) {
      mp_netlist->lock ();
    }
  }

  ~NetlistLocker ()
  {
    if (mp_netlist.get ()) {
      mp_netlist->unlock ();
    }
  }

private:
  tl::weak_ptr<Netlist> mp_netlist;
};

}

#endif
