
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

#include "gsiDecl.h"
#include "dbNetlist.h"
#include "tlException.h"
#include "tlInternational.h"

namespace gsi
{

Class<db::Pin> decl_dbPin ("db", "Pin",
  gsi::method ("id", &db::Pin::id,
    "@brief Gets the ID of the pin.\n"
  ) +
  gsi::method ("name", &db::Pin::name,
    "@brief Gets the name of the pin.\n"
  ),
  "@brief A pin of a circuit.\n"
  "Pin objects are used to describe the outgoing pins of "
  "a circuit. To create a new pin of a circuit, use \\Circuit#create_pin.\n"
  "\n"
  "This class has been added in version 0.26."
);

static void device_connect_terminal_by_name (db::Device *device, const std::string &terminal_name, db::Net *net)
{
  if (! device->device_class ()) {
    throw tl::Exception (tl::to_string (tr ("Device does not have a device class")));
  }
  size_t terminal_id = device->device_class ()->terminal_id_for_name (terminal_name);
  device->connect_terminal (terminal_id, net);
}

static void device_disconnect_terminal (db::Device *device, size_t terminal_id)
{
  device->connect_terminal (terminal_id, 0);
}

static void device_disconnect_terminal_by_name (db::Device *device, const std::string &terminal_name)
{
  device_connect_terminal_by_name (device, terminal_name, 0);
}

Class<db::Device> decl_dbDevice ("db", "Device",
  gsi::method ("device_class", &db::Device::device_class,
    "@brief Gets the device class the device belongs to.\n"
  ) +
  gsi::method ("circuit", (db::Circuit *(db::Device::*) ()) &db::Device::circuit,
    "@brief Gets the circuit the device lives in."
  ) +
  gsi::method ("id", &db::Device::id,
    "@brief Gets the device ID.\n"
    "The ID is a unique integer which identifies the device.\n"
    "It can be used to retrieve the device from the circuit using \\Circuit#device_by_id.\n"
    "When assigned, the device ID is not 0.\n"
  ) +
  gsi::method ("name=", &db::Device::set_name, gsi::arg ("name"),
    "@brief Sets the name of the device.\n"
    "Device names are used to name a device inside a netlist file. "
    "Device names should be unique within a circuit."
  ) +
  gsi::method ("name", &db::Device::name,
    "@brief Gets the name of the device.\n"
  ) +
  gsi::method ("net_for_terminal", (db::Net *(db::Device::*) (size_t)) &db::Device::net_for_terminal, gsi::arg ("terminal_id"),
    "@brief Gets the net connected to the specified terminal.\n"
    "If the terminal is not connected, nil is returned for the net."
  ) +
  gsi::method ("connect_terminal", &db::Device::connect_terminal, gsi::arg ("terminal_id"), gsi::arg ("net"),
    "@brief Connects the given terminal to the specified net.\n"
  ) +
  gsi::method_ext ("disconnect_terminal", &device_disconnect_terminal, gsi::arg ("terminal_id"),
    "@brief Disconnects the given terminal from any net.\n"
    "If the terminal has been connected to a global, this connection will be disconnected too."
  ) +
  gsi::method_ext ("connect_terminal", &device_connect_terminal_by_name, gsi::arg ("terminal_name"), gsi::arg ("net"),
    "@brief Connects the given terminal to the specified net.\n"
    "This version accepts a terminal name. If the name is not a valid terminal name, an exception is raised.\n"
    "If the terminal has been connected to a global net, it will be disconnected from there."
  ) +
  gsi::method_ext ("disconnect_terminal", &device_disconnect_terminal_by_name, gsi::arg ("terminal_name"),
    "@brief Disconnects the given terminal from any net.\n"
    "This version accepts a terminal name. If the name is not a valid terminal name, an exception is raised."
  ) +
  gsi::method ("parameter", (double (db::Device::*) (size_t) const) &db::Device::parameter_value, gsi::arg ("param_id"),
    "@brief Gets the parameter value for the given parameter ID."
  ) +
  gsi::method ("set_parameter", (void (db::Device::*) (size_t, double)) &db::Device::set_parameter_value, gsi::arg ("param_id"), gsi::arg ("value"),
    "@brief Sets the parameter value for the given parameter ID."
  ) +
  gsi::method ("parameter", (double (db::Device::*) (const std::string &) const) &db::Device::parameter_value, gsi::arg ("param_name"),
    "@brief Gets the parameter value for the given parameter name.\n"
    "If the parameter name is not valid, an exception is thrown."
  ) +
  gsi::method ("set_parameter", (void (db::Device::*) (const std::string &, double)) &db::Device::set_parameter_value, gsi::arg ("param_name"), gsi::arg ("value"),
    "@brief Sets the parameter value for the given parameter name.\n"
    "If the parameter name is not valid, an exception is thrown."
  ),
  "@brief A device inside a circuit.\n"
  "Device object represent atomic devices such as resistors, diodes or transistors. "
  "The \\Device class represents a particular device with specific parameters. "
  "The type of device is represented by a \\DeviceClass object. Device objects "
  "live in \\Circuit objects, the device class objects live in the \\Netlist object.\n"
  "\n"
  "Devices connect to nets through terminals. Terminals are described by a terminal ID which is "
  "essentially the zero-based index of the terminal. Terminal definitions can be "
  "obtained from the device class using the \\DeviceClass#terminal_definitions method.\n"
  "\n"
  "Devices connect to nets through the \\Device#connect_terminal method. "
  "Device terminals can be disconnected using \\Device#disconnect_terminal.\n"
  "\n"
  "Device objects are created inside a circuit with \\Circuit#create_device.\n"
  "\n"
  "This class has been added in version 0.26."
);

#if 0
//  TODO: activate once the geometry API is opened (clusters at al.)
Class<db::DeviceModel> decl_dbDeviceModel ("db", "DeviceModel",
  gsi::method ("netlist", (db::Netlist *(db::DeviceModel::*) ()) &db::DeviceModel::netlist,
    "@brief Gets the netlist the device model lives in."
  ) +
  gsi::method ("name=", &db::DeviceModel::set_name, gsi::arg ("name"),
    "@brief Sets the name of the device model.\n"
    "Device names are used to name a device model inside a netlist file. "
    "Device names should be unique within a netlist."
  ) +
  gsi::method ("name", &db::DeviceModel::name,
    "@brief Gets the name of the device model.\n"
  ) +
  gsi::method ("cell_index", &db::DeviceModel::cell_index,
    "@brief Gets the cell index of the device model.\n"
    "This is the cell that represents the device."
  ) +
  gsi::method ("cluster_id_for_terminal", &db::DeviceModel::cluster_id_for_terminal, gsi::arg ("terminal_id"),
    "@brief Gets the cluster ID for the given terminal.\n"
    "The cluster ID links the terminal to geometrical shapes within the clusters of the cell (see \\cell_index)"
  ),
  "@brief A geometrical device model\n"
  "This class represents the geometrical model for the device. It links into the extracted layout "
  "to a cell which holds the terminal shapes for the device.\n"
  "\n"
  "This class has been added in version 0.26."
);
#endif

static void subcircuit_connect_pin1 (db::SubCircuit *subcircuit, const db::Pin *pin, db::Net *net)
{
  if (pin) {
    subcircuit->connect_pin (pin->id (), net);
  }
}

static void subcircuit_disconnect_pin  (db::SubCircuit *subcircuit, size_t pin_id)
{
  subcircuit->connect_pin (pin_id, 0);
}

static void subcircuit_disconnect_pin1 (db::SubCircuit *subcircuit, const db::Pin *pin)
{
  if (pin) {
    subcircuit->connect_pin (pin->id (), 0);
  }
}

Class<db::SubCircuit> decl_dbSubCircuit ("db", "SubCircuit",
  gsi::method ("circuit_ref", (db::Circuit *(db::SubCircuit::*) ()) &db::SubCircuit::circuit_ref,
    "@brief Gets the circuit referenced by the subcircuit.\n"
  ) +
  gsi::method ("circuit", (db::Circuit *(db::SubCircuit::*) ()) &db::SubCircuit::circuit,
    "@brief Gets the circuit the subcircuit lives in.\n"
    "This is NOT the circuit which is referenced. For getting the circuit that the subcircuit references, use \\circuit_ref."
  ) +
  gsi::method ("id", &db::SubCircuit::id,
    "@brief Gets the subcircuit ID.\n"
    "The ID is a unique integer which identifies the subcircuit.\n"
    "It can be used to retrieve the subcircuit from the circuit using \\Circuit#subcircuit_by_id.\n"
    "When assigned, the subcircuit ID is not 0.\n"
  ) +
  gsi::method ("name=", &db::SubCircuit::set_name, gsi::arg ("name"),
    "@brief Sets the name of the subcircuit.\n"
    "SubCircuit names are used to name a subcircuits inside a netlist file. "
    "SubCircuit names should be unique within a circuit."
  ) +
  gsi::method ("name", &db::SubCircuit::name,
    "@brief Gets the name of the subcircuit.\n"
  ) +
  gsi::method ("net_for_pin", (db::Net *(db::SubCircuit::*) (size_t)) &db::SubCircuit::net_for_pin, gsi::arg ("pin_id"),
    "@brief Gets the net connected to the specified pin of the subcircuit.\n"
    "If the pin is not connected, nil is returned for the net."
  ) +
  gsi::method ("connect_pin", &db::SubCircuit::connect_pin, gsi::arg ("pin_id"), gsi::arg ("net"),
    "@brief Connects the given pin to the specified net.\n"
  ) +
  gsi::method_ext ("connect_pin", &gsi::subcircuit_connect_pin1, gsi::arg ("pin"), gsi::arg ("net"),
    "@brief Connects the given pin to the specified net.\n"
    "This version takes a \\Pin reference instead of a pin ID."
  ) +
  gsi::method_ext ("disconnect_pin", &gsi::subcircuit_disconnect_pin, gsi::arg ("pin_id"),
    "@brief Disconnects the given pin from any net.\n"
  ) +
  gsi::method_ext ("disconnect_pin", &gsi::subcircuit_disconnect_pin1, gsi::arg ("pin"),
    "@brief Disconnects the given pin from any net.\n"
    "This version takes a \\Pin reference instead of a pin ID."
  ),
  "@brief A subcircuit inside a circuit.\n"
  "Circuits may instantiate other circuits as subcircuits similar to cells "
  "in layouts. Such an instance is a subcircuit. A subcircuit refers to a "
  "circuit implementation (a \\Circuit object), and presents connections through "
  "pins. The pins of a subcircuit can be connected to nets. The subcircuit pins "
  "are identical to the outgoing pins of the circuit the subcircuit refers to.\n"
  "\n"
  "Subcircuits connect to nets through the \\SubCircuit#connect_pin method. "
  "SubCircuit pins can be disconnected using \\SubCircuit#disconnect_pin.\n"
  "\n"
  "Subcircuit objects are created inside a circuit with \\Circuit#create_subcircuit.\n"
  "\n"
  "This class has been added in version 0.26."
);

Class<db::NetTerminalRef> decl_dbNetTerminalRef ("db", "NetTerminalRef",
  gsi::method ("terminal_id", &db::NetTerminalRef::terminal_id,
    "@brief Gets the ID of the terminal of the device the connection is made to."
  ) +
  gsi::method ("device", (db::Device *(db::NetTerminalRef::*) ()) &db::NetTerminalRef::device,
    "@brief Gets the device reference.\n"
    "Gets the device object that this connection is made to."
  ) +
  gsi::method ("net", (db::Net *(db::NetTerminalRef::*) ()) &db::NetTerminalRef::net,
    "@brief Gets the net this terminal reference is attached to"
  ) +
  gsi::method ("device_class", (db::DeviceClass *(db::NetTerminalRef::*) ()) &db::NetTerminalRef::device_class,
    "@brief Gets the class of the device which is addressed."
  ) +
  gsi::method ("terminal_def", (db::DeviceTerminalDefinition *(db::NetTerminalRef::*) ()) &db::NetTerminalRef::terminal_def,
    "@brief Gets the terminal definition of the terminal that is connected"
  ),
  "@brief A connection to a terminal of a device.\n"
  "This object is used inside a net (see \\Net) to describe the connections a net makes.\n"
  "\n"
  "This class has been added in version 0.26."
);

Class<db::NetPinRef> decl_dbNetPinRef ("db", "NetPinRef",
  gsi::method ("pin_id", &db::NetPinRef::pin_id,
    "@brief Gets the ID of the pin the connection is made to."
  ) +
  gsi::method ("pin", &db::NetPinRef::pin,
    "@brief Gets the \\Pin object of the pin the connection is made to."
  ) +
  gsi::method ("net", (db::Net *(db::NetPinRef::*) ()) &db::NetPinRef::net,
    "@brief Gets the net this pin reference is attached to"
  ),
  "@brief A connection to an outgoing pin of the circuit.\n"
  "This object is used inside a net (see \\Net) to describe the connections a net makes.\n"
  "\n"
  "This class has been added in version 0.26."
);

Class<db::NetSubcircuitPinRef> decl_dbNetSubcircuitPinRef ("db", "NetSubcircuitPinRef",
  gsi::method ("pin_id", &db::NetSubcircuitPinRef::pin_id,
    "@brief Gets the ID of the pin the connection is made to."
  ) +
  gsi::method ("pin", &db::NetSubcircuitPinRef::pin,
    "@brief Gets the \\Pin object of the pin the connection is made to."
  ) +
  gsi::method ("subcircuit", (db::SubCircuit *(db::NetSubcircuitPinRef::*) ()) &db::NetSubcircuitPinRef::subcircuit,
    "@brief Gets the subcircuit reference.\n"
    "This attribute indicates the subcircuit the net attaches to. The "
    "subcircuit lives in the same circuit than the net. "
  ) +
  gsi::method ("net", (db::Net *(db::NetSubcircuitPinRef::*) ()) &db::NetSubcircuitPinRef::net,
    "@brief Gets the net this pin reference is attached to"
  ),
  "@brief A connection to a pin of a subcircuit.\n"
  "This object is used inside a net (see \\Net) to describe the connections a net makes.\n"
  "\n"
  "This class has been added in version 0.26."
);

Class<db::Net> decl_dbNet ("db", "Net",
  gsi::method ("circuit", (db::Circuit *(db::Net::*) ()) &db::Net::circuit,
    "@brief Gets the circuit the net lives in."
  ) +
  gsi::method ("clear", &db::Net::clear,
    "@brief Clears the net."
  ) +
  gsi::method ("name=", &db::Net::set_name, gsi::arg ("name"),
    "@brief Sets the name of the net.\n"
    "The name of the net is used for nameing the net in schematic files for example. "
    "The name of the net has to be unique."
  ) +
  gsi::method ("name", &db::Net::name,
    "@brief Gets the name of the net.\n"
    "See \\name= for details about the name."
  ) +
  gsi::method ("qname|to_s", &db::Net::qname,
    "@brief Gets the qualified name.\n"
    "The qualified name is like the expanded name, but the circuit's name is preceeded\n"
    "(i.e. 'CIRCUIT:NET') if available.\n"
  ) +
  gsi::method ("expanded_name", &db::Net::expanded_name,
    "@brief Gets the expanded name of the net.\n"
    "The expanded name takes the name of the net. If the name is empty, the cluster ID will be used to build a name. "
  ) +
  gsi::method ("cluster_id=", &db::Net::set_cluster_id, gsi::arg ("id"),
    "@brief Sets the cluster ID of the net.\n"
    "The cluster ID connects the net with a layout cluster. It is set when "
    "the net is extracted from a layout."
  ) +
  gsi::method ("cluster_id", &db::Net::cluster_id,
    "@brief Gets the cluster ID of the net.\n"
    "See \\cluster_id= for details about the cluster ID."
  ) +
  gsi::iterator ("each_pin", (db::Net::pin_iterator (db::Net::*) ()) &db::Net::begin_pins, (db::Net::pin_iterator (db::Net::*) ()) &db::Net::end_pins,
    "@brief Iterates over all outgoing pins the net connects.\n"
    "Pin connections are described by \\NetPinRef objects. Pin connections "
    "are connections to outgoing pins of the circuit the net lives in."
  ) +
  gsi::iterator ("each_subcircuit_pin", (db::Net::subcircuit_pin_iterator (db::Net::*) ()) &db::Net::begin_subcircuit_pins, (db::Net::subcircuit_pin_iterator (db::Net::*) ()) &db::Net::end_subcircuit_pins,
    "@brief Iterates over all subcircuit pins the net connects.\n"
    "Subcircuit pin connections are described by \\NetSubcircuitPinRef objects. These are "
    "connections to specific pins of subcircuits."
  ) +
  gsi::iterator ("each_terminal", (db::Net::terminal_iterator (db::Net::*) ()) &db::Net::begin_terminals, (db::Net::terminal_iterator (db::Net::*) ()) &db::Net::end_terminals,
    "@brief Iterates over all terminals the net connects.\n"
    "Terminals connect devices. Terminal connections are described by \\NetTerminalRef "
    "objects."
  ) +
  gsi::method ("is_floating?", &db::Net::is_floating,
    "@brief Returns true, if the net is floating.\n"
    "Floating nets are those who don't have any or only a single connection (pin_count + terminal_count < 2)."
  ) +
  gsi::method ("is_internal?", &db::Net::is_internal,
    "@brief Returns true, if the net is an internal net.\n"
    "Internal nets are those which connect exactly two terminals and nothing else (pin_count = 0 and  terminal_count == 2)."
  ) +
  gsi::method ("pin_count", &db::Net::pin_count,
    "@brief Returns the number of outgoing pins connected by this net.\n"
  ) +
  gsi::method ("subcircuit_pin_count", &db::Net::subcircuit_pin_count,
    "@brief Returns the number of subcircuit pins connected by this net.\n"
  ) +
  gsi::method ("terminal_count", &db::Net::terminal_count,
    "@brief Returns the number of terminals connected by this net.\n"
  ),
  "@brief A single net.\n"
  "A net connects multiple pins or terminals together. Pins are either "
  "pin or subcircuits of outgoing pins of the circuit the net lives in. "
  "Terminals are connections made to specific terminals of devices.\n"
  "\n"
  "Net objects are created inside a circuit with \\Circuit#create_net.\n"
  "\n"
  "To connect a net to an outgoing pin of a circuit, use \\Circuit#connect_pin, to "
  "disconnect a net from an outgoing pin use \\Circuit#disconnect_pin. "
  "To connect a net to a pin of a subcircuit, use \\SubCircuit#connect_pin, to "
  "disconnect a net from a pin of a subcircuit, use \\SubCircuit#disconnect_pin. "
  "To connect a net to a terminal of a device, use \\Device#connect_terminal, to "
  "disconnect a net from a terminal of a device, use \\Device#disconnect_terminal.\n"
  "\n"
  "This class has been added in version 0.26."
);

static db::DeviceTerminalDefinition *new_terminal_definition (const std::string &name, const std::string &description)
{
  return new db::DeviceTerminalDefinition (name, description);
}

Class<db::DeviceTerminalDefinition> decl_dbDeviceTerminalDefinition ("db", "DeviceTerminalDefinition",
  gsi::constructor ("new", &gsi::new_terminal_definition, gsi::arg ("name"), gsi::arg ("description", std::string ()),
    "@brief Creates a new terminal definition."
  ) +
  gsi::method ("name", &db::DeviceTerminalDefinition::name,
    "@brief Gets the name of the terminal."
  ) +
  gsi::method ("name=", &db::DeviceTerminalDefinition::set_name, gsi::arg ("name"),
    "@brief Sets the name of the terminal."
  ) +
  gsi::method ("description", &db::DeviceTerminalDefinition::description,
    "@brief Gets the description of the terminal."
  ) +
  gsi::method ("description=", &db::DeviceTerminalDefinition::set_description, gsi::arg ("description"),
    "@brief Sets the description of the terminal."
  ) +
  gsi::method ("id", &db::DeviceTerminalDefinition::id,
    "@brief Gets the ID of the terminal.\n"
    "The ID of the terminal is used in some places to refer to a specific terminal (e.g. in "
    "the \\NetTerminalRef object)."
  ),
  "@brief A terminal descriptor\n"
  "This class is used inside the \\DeviceClass class to describe a terminal of the device.\n"
  "\n"
  "This class has been added in version 0.26."
);

static db::DeviceParameterDefinition *new_parameter_definition (const std::string &name, const std::string &description, double default_value)
{
  return new db::DeviceParameterDefinition (name, description, default_value);
}

Class<db::DeviceParameterDefinition> decl_dbDeviceParameterDefinition ("db", "DeviceParameterDefinition",
  gsi::constructor ("new", &gsi::new_parameter_definition, gsi::arg ("name"), gsi::arg ("description", std::string ()), gsi::arg ("default_value", 0.0),
    "@brief Creates a new parameter definition."
  ) +
  gsi::method ("name", &db::DeviceParameterDefinition::name,
    "@brief Gets the name of the parameter."
  ) +
  gsi::method ("name=", &db::DeviceParameterDefinition::set_name, gsi::arg ("name"),
    "@brief Sets the name of the parameter."
  ) +
  gsi::method ("description", &db::DeviceParameterDefinition::description,
    "@brief Gets the description of the parameter."
  ) +
  gsi::method ("description=", &db::DeviceParameterDefinition::set_description, gsi::arg ("description"),
    "@brief Sets the description of the parameter."
  ) +
  gsi::method ("default_value", &db::DeviceParameterDefinition::default_value,
    "@brief Gets the default value of the parameter."
  ) +
  gsi::method ("default_value=", &db::DeviceParameterDefinition::set_default_value, gsi::arg ("default_value"),
    "@brief Sets the default value of the parameter.\n"
    "The default value is used to initialize parameters of \\Device objects."
  ) +
  gsi::method ("id", &db::DeviceParameterDefinition::id,
    "@brief Gets the ID of the parameter.\n"
    "The ID of the parameter is used in some places to refer to a specific parameter (e.g. in "
    "the \\NetParameterRef object)."
  ),
  "@brief A parameter descriptor\n"
  "This class is used inside the \\DeviceClass class to describe a parameter of the device.\n"
  "\n"
  "This class has been added in version 0.26."
);

static tl::id_type id_of_device_class (const db::DeviceClass *cls)
{
  return tl::id_of (cls);
}

Class<db::DeviceClass> decl_dbDeviceClass ("db", "DeviceClass",
  gsi::method ("name", &db::DeviceClass::name,
    "@brief Gets the name of the device class."
  ) +
  gsi::method ("description", &db::DeviceClass::description,
    "@brief Gets the description text of the device class."
  ) +
  gsi::method ("netlist", (db::Netlist *(db::DeviceClass::*) ()) &db::DeviceClass::netlist,
    "@brief Gets the netlist the device class lives in."
  ) +
  gsi::method_ext ("id", &gsi::id_of_device_class,
    "@brief Gets the unique ID of the device class\n"
    "The ID is a unique integer that identifies the device class. Use the ID "
    "to check for object identity - i.e. to determine whether two devices share the "
    "same device class."
  ) +
  gsi::method ("terminal_definitions", &db::DeviceClass::terminal_definitions,
    "@brief Gets the list of terminal definitions of the device.\n"
    "See the \\DeviceTerminalDefinition class description for details."
  ) +
  gsi::method ("terminal_definition", &db::DeviceClass::terminal_definition, gsi::arg ("terminal_id"),
    "@brief Gets the terminal definition object for a given ID.\n"
    "Terminal definition IDs are used in some places to reference a specific terminal of a device. "
    "This method obtains the corresponding definition object."
  ) +
  gsi::method ("parameter_definitions", &db::DeviceClass::parameter_definitions,
    "@brief Gets the list of parameter definitions of the device.\n"
    "See the \\DeviceParameterDefinition class description for details."
  ) +
  gsi::method ("parameter_definition", &db::DeviceClass::parameter_definition, gsi::arg ("parameter_id"),
    "@brief Gets the parameter definition object for a given ID.\n"
    "Parameter definition IDs are used in some places to reference a specific parameter of a device. "
    "This method obtains the corresponding definition object."
  ) +
  gsi::method ("has_parameter?", &db::DeviceClass::has_parameter_with_name, gsi::arg ("name"),
    "@brief Returns true, if the device class has a parameter with the given name.\n"
  ) +
  gsi::method ("parameter_id", &db::DeviceClass::parameter_id_for_name, gsi::arg ("name"),
    "@brief Returns the parameter ID of the parameter with the given name.\n"
    "An exception is thrown if there is no parameter with the given name. Use \\has_parameter to check "
    "whether the name is a valid parameter name."
  ) +
  gsi::method ("has_terminal?", &db::DeviceClass::has_terminal_with_name, gsi::arg ("name"),
    "@brief Returns true, if the device class has a terminal with the given name.\n"
  ) +
  gsi::method ("terminal_id", &db::DeviceClass::terminal_id_for_name, gsi::arg ("name"),
    "@brief Returns the terminal ID of the terminal with the given name.\n"
    "An exception is thrown if there is no terminal with the given name. Use \\has_terminal to check "
    "whether the name is a valid terminal name."
  ),
  "@brief A class describing a specific type of device.\n"
  "Device class objects live in the context of a \\Netlist object. After a "
  "device class is created, it must be added to the netlist using \\Netlist#add. "
  "The netlist will own the device class object. When the netlist is destroyed, the "
  "device class object will become invalid.\n"
  "\n"
  "The \\DeviceClass class is the base class for other device classes.\n"
  "\n"
  "This class has been added in version 0.26."
);

namespace {

/**
 *  @brief A DeviceClass implementation that allows reimplementation of the virtual methods
 *
 *  NOTE: cloning of the generic device class is not supported currently. Hence when the
 *  netlist is copied, the device class attributes will remain, but the functionality is lost.
 */
class GenericDeviceClass
  : public db::DeviceClass
{
public:
  GenericDeviceClass ()
    : db::DeviceClass (), m_supports_parallel_combination (true), m_supports_serial_combination (true)
  {
    //  .. nothing yet ..
  }

  virtual bool combine_devices (db::Device *a, db::Device *b) const
  {
    if (cb_combine_devices.can_issue ()) {
      return cb_combine_devices.issue<db::DeviceClass, bool, db::Device *, db::Device *> (&db::DeviceClass::combine_devices, a, b);
    } else {
      return db::DeviceClass::combine_devices (a, b);
    }
  }

  virtual bool supports_parallel_combination () const
  {
    return m_supports_parallel_combination;
  }

  virtual bool supports_serial_combination () const
  {
    return m_supports_serial_combination;
  }

  void set_supports_parallel_combination (bool f)
  {
    m_supports_parallel_combination = f;
  }

  void set_supports_serial_combination (bool f)
  {
    m_supports_serial_combination = f;
  }

  gsi::Callback cb_combine_devices;

private:
  bool m_supports_parallel_combination;
  bool m_supports_serial_combination;
};

}

static void gdc_add_terminal_definition (GenericDeviceClass *cls, db::DeviceTerminalDefinition *terminal_def)
{
  if (terminal_def) {
    *terminal_def = cls->add_terminal_definition (*terminal_def);
  }
}

static void gdc_add_parameter_definition (GenericDeviceClass *cls, db::DeviceParameterDefinition *parameter_def)
{
  if (parameter_def) {
    *parameter_def = cls->add_parameter_definition (*parameter_def);
  }
}

Class<GenericDeviceClass> decl_GenericDeviceClass (decl_dbDeviceClass, "db", "GenericDeviceClass",
  gsi::method_ext ("add_terminal", &gsi::gdc_add_terminal_definition, gsi::arg ("terminal_def"),
    "@brief Adds the given terminal definition to the device class\n"
    "This method will define a new terminal. The new terminal is added at the end of existing terminals. "
    "The terminal definition object passed as the argument is modified to contain the "
    "new ID of the terminal.\n"
    "\n"
    "The terminal is copied into the device class. Modifying the terminal object later "
    "does not have the effect of changing the terminal definition."
  ) +
  gsi::method ("clear_terminals", &GenericDeviceClass::clear_terminal_definitions,
    "@brief Clears the list of terminals\n"
  ) +
  gsi::method_ext ("add_parameter", &gsi::gdc_add_parameter_definition, gsi::arg ("parameter_def"),
    "@brief Adds the given parameter definition to the device class\n"
    "This method will define a new parameter. The new parameter is added at the end of existing parameters. "
    "The parameter definition object passed as the argument is modified to contain the "
    "new ID of the parameter."
    "\n"
    "The parameter is copied into the device class. Modifying the parameter object later "
    "does not have the effect of changing the parameter definition."
  ) +
  gsi::method ("clear_parameters", &GenericDeviceClass::clear_parameter_definitions,
    "@brief Clears the list of parameters\n"
  ) +
  gsi::method ("name=", &GenericDeviceClass::set_name, gsi::arg ("name"),
    "@brief Sets the name of the device\n"
  ) +
  gsi::method ("description=", &GenericDeviceClass::set_description, gsi::arg ("description"),
    "@brief Sets the description of the device\n"
  ) +
  gsi::callback ("combine_devices", &GenericDeviceClass::combine_devices, &GenericDeviceClass::cb_combine_devices, gsi::arg ("a"), gsi::arg ("b"),
    "@brief Combines two devices.\n"
    "This method shall test, whether the two devices can be combined. Both devices "
    "are guaranteed to share the same device class (self). "
    "If they cannot be combined, this method shall do nothing and return false. "
    "If they can be combined, this method shall reconnect the nets of the first "
    "device and entirely disconnect the nets of the second device. "
    "It shall combine the parameters of both devices into the first. "
    "The second device will be deleted afterwards.\n"
  ) +
  gsi::method ("supports_parallel_combination=", &GenericDeviceClass::set_supports_parallel_combination, gsi::arg ("f"),
    "@brief Specifies whether the device supports parallel device combination.\n"
    "Parallel device combination means that all terminals of two combination candidates are connected to the same nets. "
    "If the device does not support this combination mode, this predicate can be set to false. This will make the device "
    "extractor skip the combination test in parallel mode and improve performance somewhat."
  ) +
  gsi::method ("supports_serial_combination=", &GenericDeviceClass::set_supports_serial_combination, gsi::arg ("f"),
    "@brief Specifies whether the device supports serial device combination.\n"
    "Serial device combination means that the devices are connected by internal nodes. "
    "If the device does not support this combination mode, this predicate can be set to false. This will make the device "
    "extractor skip the combination test in serial mode and improve performance somewhat."
  ),
  "@brief A generic device class\n"
  "This class allows building generic device classes. Specificially, terminals can be defined "
  "by adding terminal definitions. Terminal definitions should not be added dynamically. To create "
  "your own device, instantiate the \\GenericDeviceClass object, set name and description and "
  "specify the terminals. Then add this new device class to the \\Netlist object where it will live "
  "and be used to define device instances (\\Device objects).\n"
  "\n"
  "In addition, parameters can be defined which correspond to values stored inside the "
  "specific device instance (\\Device object)."
  "\n"
  "This class has been added in version 0.26."
);

static db::Net *create_net (db::Circuit *c, const std::string &name)
{
  db::Net *n = new db::Net ();
  c->add_net (n);
  n->set_name (name);
  return n;
}

static db::Device *create_device1 (db::Circuit *c, db::DeviceClass *dc, const std::string &name)
{
  db::Device *d = new db::Device (dc, name);
  c->add_device (d);
  return d;
}

static db::SubCircuit *create_subcircuit1 (db::Circuit *c, db::Circuit *cc, const std::string &name)
{
  db::SubCircuit *sc = new db::SubCircuit (cc, name);
  c->add_subcircuit (sc);
  return sc;
}

static db::Net *circuit_net_for_pin (db::Circuit *c, const db::Pin *pin)
{
  return pin ? c->net_for_pin (pin->id ()) : 0;
}

static void circuit_connect_pin1 (db::Circuit *c, const db::Pin *pin, db::Net *net)
{
  if (pin) {
    c->connect_pin (pin->id (), net);
  }
}

static void circuit_disconnect_pin (db::Circuit *c, size_t pin_id)
{
  c->connect_pin (pin_id, 0);
}

static void circuit_disconnect_pin1 (db::Circuit *c, const db::Pin *pin)
{
  if (pin) {
    c->connect_pin (pin->id (), 0);
  }
}

Class<db::Circuit> decl_dbCircuit ("db", "Circuit",
  gsi::method ("create_pin", &db::Circuit::add_pin, gsi::arg ("name"),
    "@brief Creates a new \\Pin object inside the circuit\n"
    "This object will describe a pin of the circuit. A circuit connects "
    "to the outside through such a pin. The pin is added after all existing "
    "pins. For more details see the \\Pin class."
  ) +
  gsi::iterator ("each_child", (db::Circuit::child_circuit_iterator (db::Circuit::*) ()) &db::Circuit::begin_children, (db::Circuit::child_circuit_iterator (db::Circuit::*) ()) &db::Circuit::end_children,
    "@brief Iterates over the child circuits of this circuit\n"
    "Child circuits are the ones that are referenced from this circuit via subcircuits."
  ) +
  gsi::iterator ("each_parent", (db::Circuit::parent_circuit_iterator (db::Circuit::*) ()) &db::Circuit::begin_parents, (db::Circuit::parent_circuit_iterator (db::Circuit::*) ()) &db::Circuit::end_parents,
    "@brief Iterates over the parent circuits of this circuit\n"
    "Child circuits are the ones that are referencing this circuit via subcircuits."
  ) +
  gsi::iterator ("each_ref", (db::Circuit::refs_iterator (db::Circuit::*) ()) &db::Circuit::begin_refs, (db::Circuit::refs_iterator (db::Circuit::*) ()) &db::Circuit::end_refs,
    "@brief Iterates over the subcircuit objects referencing this circuit\n"
  ) +
  gsi::iterator ("each_pin", (db::Circuit::pin_iterator (db::Circuit::*) ()) &db::Circuit::begin_pins, (db::Circuit::pin_iterator (db::Circuit::*) ()) &db::Circuit::end_pins,
    "@brief Iterates over the pins of the circuit"
  ) +
  gsi::method ("device_by_id", (db::Device *(db::Circuit::*) (size_t)) &db::Circuit::device_by_id, gsi::arg ("id"),
    "@brief Gets the device object for a given ID.\n"
    "If the ID is not a valid device ID, nil is returned."
  ) +
  gsi::method ("device_by_name", (db::Device *(db::Circuit::*) (const std::string &)) &db::Circuit::device_by_name, gsi::arg ("name"),
    "@brief Gets the device object for a given name.\n"
    "If the ID is not a valid device name, nil is returned."
  ) +
  gsi::method ("subcircuit_by_id", (db::SubCircuit *(db::Circuit::*) (size_t)) &db::Circuit::subcircuit_by_id, gsi::arg ("id"),
    "@brief Gets the subcircuit object for a given ID.\n"
    "If the ID is not a valid subcircuit ID, nil is returned."
  ) +
  gsi::method ("subcircuit_by_name", (db::SubCircuit *(db::Circuit::*) (const std::string &)) &db::Circuit::subcircuit_by_name, gsi::arg ("name"),
    "@brief Gets the subcircuit object for a given name.\n"
    "If the ID is not a valid subcircuit name, nil is returned."
  ) +
  gsi::method ("net_by_cluster_id", (db::Net *(db::Circuit::*) (size_t)) &db::Circuit::net_by_cluster_id, gsi::arg ("cluster_id"),
    "@brief Gets the net object corresponding to a specific cluster ID\n"
    "If the ID is not a valid pin cluster ID, nil is returned."
  ) +
  gsi::method ("net_by_name", (db::Net *(db::Circuit::*) (const std::string &)) &db::Circuit::net_by_name, gsi::arg ("name"),
    "@brief Gets the net object for a given name.\n"
    "If the ID is not a valid net name, nil is returned."
  ) +
  gsi::method ("pin_by_id", &db::Circuit::pin_by_id, gsi::arg ("id"),
    "@brief Gets the \\Pin object corresponding to a specific ID\n"
    "If the ID is not a valid pin ID, nil is returned."
  ) +
  gsi::method ("pin_by_name", &db::Circuit::pin_by_name, gsi::arg ("name"),
    "@brief Gets the \\Pin object corresponding to a specific name\n"
    "If the ID is not a valid pin name, nil is returned."
  ) +
  gsi::method ("pin_count", &db::Circuit::pin_count,
    "@brief Gets the number of pins in the circuit"
  ) +
  gsi::method_ext ("create_net", &gsi::create_net, gsi::arg ("name", std::string ()),
    "@brief Creates a new \\Net object inside the circuit\n"
    "This object will describe a net of the circuit. The nets are basically "
    "connections between the different components of the circuit (subcircuits, "
    "devices and pins).\n"
    "\n"
    "A net needs to be filled with references to connect to specific objects. "
    "See the \\Net class for more details."
  ) +
  gsi::method ("remove_net", &db::Circuit::remove_net, gsi::arg ("net"),
    "@brief Removes the given net from the circuit\n"
  ) +
  gsi::iterator ("each_net", (db::Circuit::net_iterator (db::Circuit::*) ()) &db::Circuit::begin_nets, (db::Circuit::net_iterator (db::Circuit::*) ()) &db::Circuit::end_nets,
    "@brief Iterates over the nets of the circuit"
  ) +
  gsi::method_ext ("create_device", &gsi::create_device1, gsi::arg ("device_class"), gsi::arg ("name", std::string ()),
    "@brief Creates a new bound \\Device object inside the circuit\n"
    "This object describes a device of the circuit. The device is already attached "
    "to the device class. The name is optional and is used to identify the device in a "
    "netlist file.\n"
    "\n"
    "For more details see the \\Device class."
  ) +
  gsi::method ("remove_device", &db::Circuit::remove_device, gsi::arg ("device"),
    "@brief Removes the given device from the circuit\n"
  ) +
  gsi::iterator ("each_device", (db::Circuit::device_iterator (db::Circuit::*) ()) &db::Circuit::begin_devices, (db::Circuit::device_iterator (db::Circuit::*) ()) &db::Circuit::end_devices,
    "@brief Iterates over the devices of the circuit"
  ) +
  gsi::method_ext ("create_subcircuit", &gsi::create_subcircuit1, gsi::arg ("circuit"), gsi::arg ("name", std::string ()),
    "@brief Creates a new bound \\SubCircuit object inside the circuit\n"
    "This object describes an instance of another circuit inside the circuit. The subcircuit is already attached "
    "to the other circuit. The name is optional and is used to identify the subcircuit in a "
    "netlist file.\n"
    "\n"
    "For more details see the \\SubCircuit class."
  ) +
  gsi::method ("remove_subcircuit", &db::Circuit::remove_subcircuit, gsi::arg ("subcircuit"),
    "@brief Removes the given subcircuit from the circuit\n"
  ) +
  gsi::iterator ("each_subcircuit", (db::Circuit::subcircuit_iterator (db::Circuit::*) ()) &db::Circuit::begin_subcircuits, (db::Circuit::subcircuit_iterator (db::Circuit::*) ()) &db::Circuit::end_subcircuits,
    "@brief Iterates over the subcircuits of the circuit"
  ) +
  gsi::method ("netlist", (db::Netlist *(db::Circuit::*) ()) &db::Circuit::netlist,
    "@brief Gets the netlist object the circuit lives in"
  ) +
  gsi::method ("name=", &db::Circuit::set_name, gsi::arg ("name"),
    "@brief Sets the name of the circuit"
  ) +
  gsi::method ("name", &db::Circuit::name,
    "@brief Gets the name of the circuit"
  ) +
  gsi::method ("cell_index=", &db::Circuit::set_cell_index, gsi::arg ("cell_index"),
    "@brief Sets the cell index\n"
    "The cell index relates a circuit with a cell from a layout. It's intended to "
    "hold a cell index number if the netlist was extracted from a layout.\n"
  ) +
  gsi::method ("cell_index", &db::Circuit::cell_index,
    "@brief Gets the cell index of the circuit\n"
    "See \\cell_index= for details.\n"
  ) +
  gsi::method ("net_for_pin", (db::Net *(db::Circuit::*) (size_t)) &db::Circuit::net_for_pin, gsi::arg ("pin_id"),
    "@brief Gets the net object attached to a specific pin.\n"
    "This is the net object inside the circuit which attaches to the given outward-bound pin.\n"
    "This method returns nil if the pin is not connected or the pin ID is invalid."
  ) +
  gsi::method_ext ("net_for_pin", &gsi::circuit_net_for_pin, gsi::arg ("pin"),
    "@brief Gets the net object attached to a specific pin.\n"
    "This is the net object inside the circuit which attaches to the given outward-bound pin.\n"
    "This method returns nil if the pin is not connected or the pin object is nil."
  ) +
  gsi::method ("connect_pin", &db::Circuit::connect_pin, gsi::arg ("pin_id"), gsi::arg ("net"),
    "@brief Connects the given pin with the given net.\n"
    "The net must be one inside the circuit. Any previous connected is resolved before this "
    "connection is made. A pin can only be connected to one net at a time."
  ) +
  gsi::method_ext ("connect_pin", &gsi::circuit_connect_pin1, gsi::arg ("pin"), gsi::arg ("net"),
    "@brief Connects the given pin with the given net.\n"
    "The net and the pin must be objects from inside the circuit. Any previous connected is resolved before this "
    "connection is made. A pin can only be connected to one net at a time."
  ) +
  gsi::method_ext ("disconnect_pin", &gsi::circuit_disconnect_pin, gsi::arg ("pin_id"),
    "@brief Disconnects the given pin from any net.\n"
  ) +
  gsi::method_ext ("disconnect_pin", &gsi::circuit_disconnect_pin1, gsi::arg ("pin"),
    "@brief Disconnects the given pin from any net.\n"
  ) +
  gsi::method ("clear", &db::Circuit::clear,
    "@brief Clears the circuit\n"
    "This method removes all objects and clears the other attributes."
  ) +
  gsi::method ("combine_devices", &db::Circuit::combine_devices,
    "@brief Combines devices where possible\n"
    "This method will combine devices that can be combined according "
    "to their device classes 'combine_devices' method.\n"
    "For example, serial or parallel resistors can be combined into "
    "a single resistor.\n"
  ) +
  gsi::method ("purge_nets", &db::Circuit::purge_nets,
    "@brief Purges floating nets.\n"
    "Floating nets can be created as effect of reconnections of devices or pins. "
    "This method will eliminate all nets that make less than two connections."
  ),
  "@brief Circuits are the basic building blocks of the netlist\n"
  "A circuit has pins by which it can connect to the outside. Pins are "
  "created using \\create_pin and are represented by the \\Pin class.\n"
  "\n"
  "Futhermore, a circuit manages the components of the netlist. "
  "Components are devices (class \\Device) and subcircuits (class \\SubCircuit). "
  "Devices are basic devices such as resistors or transistors. Subcircuits "
  "are other circuits to which nets from this circuit connect. "
  "Devices are created using the \\create_device method. Subcircuits are "
  "created using the \\create_subcircuit method.\n"
  "\n"
  "Devices are connected through 'terminals', subcircuits are connected through "
  "their pins. Terminals and pins are described by integer ID's in the context of "
  "most methods.\n"
  "\n"
  "Finally, the circuit consists of the nets. Nets connect terminals of devices "
  "and pins of subcircuits or the circuit itself. Nets are created using "
  "\\create_net and are represented by objects of the \\Net class.\n"
  "See there for more about nets.\n"
  "\n"
  "The Circuit object is only valid if the netlist object is alive. "
  "Circuits must be added to a netlist using \\Netlist#add to become "
  "part of the netlist.\n"
  "\n"
  "The Circuit class has been introduced in version 0.26."
);

static void add_circuit (db::Netlist *nl, db::Circuit *c)
{
  c->keep ();
  nl->add_circuit (c);
}

static void add_device_class (db::Netlist *nl, db::DeviceClass *cl)
{
  cl->keep ();
  nl->add_device_class (cl);
}

Class<db::Netlist> decl_dbNetlist ("db", "Netlist",
  gsi::method_ext ("add", &gsi::add_circuit,
    "@brief Adds the circuit to the netlist\n"
    "This method will add the given circuit object to the netlist. "
    "After the circuit has been added, it will be owned by the netlist."
  ) +
  gsi::method ("remove", &db::Netlist::remove_circuit, gsi::arg ("circuit"),
    "@brief Removes the given circuit object from the netlist\n"
    "After the object has been removed, it becomes invalid and cannot be used further."
  ) +
  gsi::method ("circuit_by_cell_index", (db::Circuit *(db::Netlist::*) (db::cell_index_type)) &db::Netlist::circuit_by_cell_index, gsi::arg ("cell_index"),
    "@brief Gets the circuit object for a given cell index.\n"
    "If the cell index is not valid or no circuit is registered with this index, nil is returned."
  ) +
  gsi::method ("circuit_by_name", (db::Circuit *(db::Netlist::*) (const std::string &)) &db::Netlist::circuit_by_name, gsi::arg ("name"),
    "@brief Gets the circuit object for a given name.\n"
    "If the ID is not a valid circuit name, nil is returned."
  ) +
  gsi::iterator ("each_circuit_top_down", (db::Netlist::top_down_circuit_iterator (db::Netlist::*) ()) &db::Netlist::begin_top_down, (db::Netlist::top_down_circuit_iterator (db::Netlist::*) ()) &db::Netlist::end_top_down,
    "@brief Iterates over the circuits top-down\n"
    "Iterating top-down means the parent circuits come before the child circuits. "
    "The first \\top_circuit_count circuits are top circuits - i.e. those which are not referenced by other circuits."
  ) +
  gsi::iterator ("each_circuit_bottom_up", (db::Netlist::bottom_up_circuit_iterator (db::Netlist::*) ()) &db::Netlist::begin_bottom_up, (db::Netlist::bottom_up_circuit_iterator (db::Netlist::*) ()) &db::Netlist::end_bottom_up,
    "@brief Iterates over the circuits bottom-up\n"
    "Iterating bottom-up means the parent circuits come after the child circuits. "
    "This is the basically the reverse order as delivered by \\each_circuit_top_down."
  ) +
  gsi::method ("top_circuit_count", &db::Netlist::top_circuit_count,
    "@brief Gets the number of top circuits.\n"
    "Top circuits are those which are not referenced by other circuits via subcircuits. "
    "A well-formed netlist has a single top circuit."
  ) +
  gsi::iterator ("each_circuit", (db::Netlist::circuit_iterator (db::Netlist::*) ()) &db::Netlist::begin_circuits, (db::Netlist::circuit_iterator (db::Netlist::*) ()) &db::Netlist::end_circuits,
    "@brief Iterates over the circuits of the netlist"
  ) +
  gsi::method_ext ("add", &gsi::add_device_class,
    "@brief Adds the device class to the netlist\n"
    "This method will add the given device class object to the netlist. "
    "After the device class has been added, it will be owned by the netlist."
  ) +
  gsi::method ("remove", &db::Netlist::remove_device_class, gsi::arg ("device_class"),
    "@brief Removes the given device class object from the netlist\n"
    "After the object has been removed, it becomes invalid and cannot be used further. "
    "Use this method with care as it may corrupt the internal structure of the netlist. "
    "Only use this method when device refers to this device class."
  ) +
  gsi::iterator ("each_device_class", (db::Netlist::device_class_iterator (db::Netlist::*) ()) &db::Netlist::begin_device_classes, (db::Netlist::device_class_iterator (db::Netlist::*) ()) &db::Netlist::end_device_classes,
    "@brief Iterates over the device classes of the netlist"
  ) +
  gsi::method ("to_s", &db::Netlist::to_string,
    "@brief Converts the netlist to a string representation.\n"
    "This method is intended for test purposes mainly."
  ) +
  gsi::method ("combine_devices", &db::Netlist::combine_devices,
    "@brief Combines devices where possible\n"
    "This method will combine devices that can be combined according "
    "to their device classes 'combine_devices' method.\n"
    "For example, serial or parallel resistors can be combined into "
    "a single resistor.\n"
  ) +
  gsi::method ("make_top_level_pins", &db::Netlist::make_top_level_pins,
    "@brief Creates pins for top-level circuits.\n"
    "This method will turn all named nets of top-level circuits (such that are not "
    "referenced by subcircuits) into pins. This method can be used before purge to "
    "avoid that purge will remove nets which are directly connecting to subcircuits."
  ) +
  gsi::method ("purge", &db::Netlist::purge,
    "@brief Purge unused nets, circuits and subcircuits.\n"
    "This method will purge all nets which return \\floating == true. Circuits which don't have any "
    "nets (or only floating ones) and removed. Their subcircuits are disconnected."
  ) +
  gsi::method ("purge_nets", &db::Netlist::purge_nets,
    "@brief Purges floating nets.\n"
    "Floating nets can be created as effect of reconnections of devices or pins. "
    "This method will eliminate all nets that make less than two connections."
  ),
  "@brief The netlist top-level class\n"
  "A netlist is a hierarchical structure of circuits. At least one circuit is the "
  "top-level circuit, other circuits may be referenced as subcircuits.\n"
  "Circuits are created with \\create_circuit and are represented by objects of the \\Circuit class.\n"
  "\n"
  "Beside circuits, the netlist manages device classes. Device classes describe specific "
  "types of devices. Device classes are represented by objects of the \\DeviceClass class "
  "and are created using \\create_device_class.\n"
  "\n"
  "The netlist class has been introduced with version 0.26."
);

}
