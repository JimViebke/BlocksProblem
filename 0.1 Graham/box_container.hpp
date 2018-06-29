#include <vector>
#include <memory>
#include <algorithm>

#include "consts.hpp"
#include "bucket.hpp"
#include "box.hpp"

namespace uva
{
	class box_container
	{
	public:
		using box_lookup_type = std::vector<box>;
		using bucket_lookup_type = std::vector<bucket>;
		using layout_type = std::vector<size_t>;
	private:
		size_t bucket_size;
		box_lookup_type box_lookup;
		bucket_lookup_type bucket_lookup;
		layout_type layout;

		bool is_valid_command(const size_t &a_idx, const size_t &b_idx)
		{
			return box_lookup[a_idx].bucket() != box_lookup[b_idx].bucket();
		}

		void return_boxes_above(const size_t &idx)
		{
			auto end_of_boxes = bucket_lookup[box_lookup[idx].bucket()].next();
			for(size_t above_box = box_lookup[idx].index() + 1; above_box < end_of_boxes; ++above_box)
			{
				bucket_lookup[box_lookup[idx].bucket()].next(bucket_lookup[box_lookup[idx].bucket()].next() - 1);

				box_lookup[layout[above_box]].index(bucket_lookup[layout[above_box]].begin());
				box_lookup[layout[above_box]].bucket(layout[above_box]);

				layout[box_lookup[layout[above_box]].index()] = layout[above_box];
				layout[above_box] = NULL_PLACEHOLDER;
			}
		}

		void move(const size_t a_idx, const size_t &b_idx)
		{
			layout[bucket_lookup[box_lookup[b_idx].bucket()].next()] = a_idx;
			layout[box_lookup[a_idx].index()] = NULL_PLACEHOLDER;

			bucket_lookup[box_lookup[b_idx].bucket()].next(bucket_lookup[box_lookup[b_idx].bucket()].next() + 1);

			if(bucket_lookup[box_lookup[a_idx].bucket()].next() - 1 != bucket_lookup[box_lookup[a_idx].bucket()].begin())
				bucket_lookup[box_lookup[a_idx].bucket()].next(bucket_lookup[box_lookup[a_idx].bucket()].next() - 1);

			box_lookup[a_idx].index(bucket_lookup[box_lookup[b_idx].bucket()].next() - 1);
			box_lookup[a_idx].bucket(box_lookup[b_idx].bucket());
		}

		void pile(const size_t &a_idx, const size_t &b_idx)
		{
			auto end_of_boxes = bucket_lookup[box_lookup[a_idx].bucket()].next();

			for(size_t a_idx_pile = box_lookup[a_idx].index(); a_idx_pile < end_of_boxes; ++a_idx_pile)
				move(layout[a_idx_pile], b_idx);
		}

	public:
		box_container(const size_t &n) : bucket_size(n + 1), layout((n * n) + n, NULL_PLACEHOLDER)
		{
			box_lookup.reserve(n);
			bucket_lookup.reserve(n);
			for (size_t lookup_idx = 0, layout_idx = 0; lookup_idx < n; ++lookup_idx, layout_idx += n + 1)
			{
				box_lookup.push_back(box(layout_idx, lookup_idx));
				bucket_lookup.push_back(bucket(layout_idx, layout_idx + n, layout_idx + 1));
				layout[layout_idx] = lookup_idx;
			}
		}

		void move_onto(const size_t &a_idx, const size_t &b_idx)
		{
			if (is_valid_command(a_idx, b_idx))
			{
				return_boxes_above(a_idx);
				return_boxes_above(b_idx);
				move(a_idx, b_idx);
			}
		}

		void move_over(const size_t &a_idx, const size_t &b_idx)
		{
			if (is_valid_command(a_idx, b_idx))
			{
				return_boxes_above(a_idx);
				move(a_idx, b_idx);
			}
		}

		void pile_onto(const size_t &a_idx, const size_t &b_idx)
		{
			if (is_valid_command(a_idx, b_idx))
			{
				return_boxes_above(b_idx);
				pile(a_idx, b_idx);
			}
		}

		void pile_over(const size_t &a_idx, const size_t &b_idx)
		{
			if (is_valid_command(a_idx, b_idx))
				pile(a_idx, b_idx);
		}

		layout_type get_layout() { return layout; }
	};
}
