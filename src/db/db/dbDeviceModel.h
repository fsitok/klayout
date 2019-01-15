
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

#ifndef _HDR_dbDeviceModel
#define _HDR_dbDeviceModel

#include "dbCommon.h"
#include "dbNet.h"
#include "dbPoint.h"

#include "tlObject.h"

#include <vector>

namespace db
{

class Netlist;

/**
 *  @brief A device model
 *
 *  A device model represents the geometrical properties of a device. It basically links
 *  to a cell and clusters for indicating the terminal geometry of the device.
 */
class DB_PUBLIC DeviceModel
  : public tl::Object
{
public:
  /**
   *  @brief The constructor
   */
  DeviceModel (const std::string &name = std::string ());

  /**
   *  @brief Copy constructor
   */
  DeviceModel (const DeviceModel &other);

  /**
   *  @brief Assignment
   */
  DeviceModel &operator= (const DeviceModel &other);

  /**
   *  @brief Destructor
   */
  ~DeviceModel ();

  /**
   *  @brief Gets the netlist the device lives in (const version)
   *  This pointer is 0 if the device model isn't added to a netlist
   */
  const Netlist *netlist () const
  {
    return mp_netlist;
  }

  /**
   *  @brief Gets the netlist the device lives in (non-const version)
   *  This pointer is 0 if the device model isn't added to a netlist
   */
  Netlist *netlist ()
  {
    return mp_netlist;
  }

  /**
   *  @brief Sets the name
   */
  void set_name (const std::string &n);

  /**
   *  @brief Gets the name
   */
  const std::string &name () const
  {
    return m_name;
  }

  /**
   *  @brief Sets the device cell index
   *  In the layout, a device is represented by a cell. This attribute gives the index of this
   *  cell.
   */
  void set_cell_index (db::cell_index_type ci);

  /**
   *  @brief Gets the device cell index
   */
  db::cell_index_type cell_index () const
  {
    return m_cell_index;
  }

  /**
   *  @brief Gets the cluster ID for a given terminal
   *  This attribute connects the device terminal with a terminal cluster
   */
  size_t cluster_id_for_terminal (size_t terminal_id) const;

  /**
   *  @brief Sets the cluster ID for a given terminal
   */
  void set_cluster_id_for_terminal (size_t terminal_id, size_t cluster_id);

private:
  friend class Netlist;

  std::string m_name;
  db::cell_index_type m_cell_index;
  std::vector<size_t> m_terminal_cluster_ids;
  Netlist *mp_netlist;

  /**
   *  @brief Sets the netlist
   */
  void set_netlist (Netlist *netlist);
};

}

#endif