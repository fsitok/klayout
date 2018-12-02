
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2018 Matthias Koefferlein

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


#ifndef HDR_dbEmptyRegion
#define HDR_dbEmptyRegion

#include "dbCommon.h"
#include "dbRegionDelegate.h"

namespace db {

/**
 *  @brief An empty Region
 */
class DB_PUBLIC EmptyRegion
  : public RegionDelegate
{
public:
  EmptyRegion ();
  virtual ~EmptyRegion ();

  EmptyRegion (const EmptyRegion &other);
  RegionDelegate *clone () const;

  virtual RegionIteratorDelegate *begin () const { return 0; }
  virtual RegionIteratorDelegate *begin_merged () const { return 0; }

  virtual std::pair<db::RecursiveShapeIterator, db::ICplxTrans> begin_iter () const { return std::make_pair (db::RecursiveShapeIterator (), db::ICplxTrans ()); }
  virtual std::pair<db::RecursiveShapeIterator, db::ICplxTrans> begin_merged_iter () const { return std::make_pair (db::RecursiveShapeIterator (), db::ICplxTrans ()); }

  virtual bool empty () const { return true; }
  virtual size_t size () const { return 0; }
  virtual std::string to_string (size_t) const { return std::string (); }

  virtual bool is_box () const { return false; }
  virtual bool is_merged () const { return true; }
  virtual area_type area (const db::Box &) const { return 0; }
  virtual perimeter_type perimeter (const db::Box &) const { return 0; }

  virtual Box bbox () const { return Box (); }

  virtual EdgePairs width_check (db::Coord, bool, metrics_type, double, distance_type, distance_type) const { return EdgePairs (); }
  virtual EdgePairs space_check (db::Coord, bool, metrics_type, double, distance_type, distance_type) const { return EdgePairs (); }
  virtual EdgePairs isolated_check (db::Coord, bool, metrics_type, double, distance_type, distance_type) const { return EdgePairs (); }
  virtual EdgePairs notch_check (db::Coord, bool, metrics_type, double, distance_type, distance_type) const { return EdgePairs (); }
  virtual EdgePairs enclosing_check (const Region &, db::Coord, bool, metrics_type, double, distance_type, distance_type) const { return EdgePairs (); }
  virtual EdgePairs overlap_check (const Region &, db::Coord, bool, metrics_type, double, distance_type, distance_type) const { return EdgePairs (); }
  virtual EdgePairs separation_check (const Region &, db::Coord, bool , metrics_type, double, distance_type, distance_type) const { return EdgePairs (); }
  virtual EdgePairs inside_check (const Region &, db::Coord, bool, metrics_type, double, distance_type, distance_type) const { return EdgePairs (); }
  virtual EdgePairs grid_check (db::Coord, db::Coord) const { return EdgePairs (); }
  virtual EdgePairs angle_check (double, double, bool) const { return EdgePairs (); }

  virtual RegionDelegate *snapped_in_place (db::Coord, db::Coord) { return this; }
  virtual RegionDelegate *snapped (db::Coord, db::Coord)  { return new EmptyRegion (); }

  virtual RegionDelegate *strange_polygon_check () const { return new EmptyRegion (); }

  virtual Edges edges (const EdgeFilterBase *) const { return db::Edges (); }
  virtual RegionDelegate *filter_in_place (const PolygonFilterBase &) { return this; }
  virtual RegionDelegate *filtered (const PolygonFilterBase &) const { return new EmptyRegion (); }

  virtual RegionDelegate *merged_in_place () { return this; }
  virtual RegionDelegate *merged_in_place (bool, unsigned int) { return this; }
  virtual RegionDelegate *merged () const { return new EmptyRegion (); }
  virtual RegionDelegate *merged (bool, unsigned int) const { return new EmptyRegion (); }

  virtual RegionDelegate *sized (coord_type, unsigned int) const { return new EmptyRegion (); }
  virtual RegionDelegate *sized (coord_type, coord_type, unsigned int) const { return new EmptyRegion (); }

  virtual RegionDelegate *and_with (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *not_with (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *xor_with (const Region &other) const;
  virtual RegionDelegate *or_with (const Region &other) const;
  virtual RegionDelegate *add_in_place (const Region &other);
  virtual RegionDelegate *add (const Region &other) const;

  virtual RegionDelegate *selected_outside (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_not_outside (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_inside (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_not_inside (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_interacting (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_not_interacting (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_interacting (const Edges &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_not_interacting (const Edges &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_overlapping (const Region &) const { return new EmptyRegion (); }
  virtual RegionDelegate *selected_not_overlapping (const Region &) const { return new EmptyRegion (); }

  virtual RegionDelegate *holes () const { return new EmptyRegion (); }
  virtual RegionDelegate *hulls () const { return new EmptyRegion (); }
  virtual RegionDelegate *in (const Region &, bool) const { return new EmptyRegion (); }
  virtual RegionDelegate *rounded_corners (double, double, unsigned int) const { return new EmptyRegion (); }
  virtual RegionDelegate *smoothed (coord_type) const { return new EmptyRegion (); }

  virtual bool has_valid_polygons () const { return true; }
  virtual bool has_valid_merged_polygons () const { return true; }
  virtual const db::Polygon *nth (size_t) const { tl_assert (false); }

  virtual const db::RecursiveShapeIterator *iter () const { return 0; }

  virtual bool equals (const Region &other) const;
  virtual bool less (const Region &other) const;

  virtual void insert_into (Layout *, db::cell_index_type, unsigned int) const { }

private:
  EmptyRegion &operator= (const EmptyRegion &other);
};

}  // namespace db

#endif
