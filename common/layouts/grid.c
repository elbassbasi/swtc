/*
 * layoutgrid.c
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

void w_layout_grid_do_layout_0(w_layout_grid *layout, w_composite *composite,
		int move, w_rect *r, w_size *sz, int flushCache);
/*
 * public function
 */
void w_layout_grid_data_init(w_layout_grid_data *griddata) {
	((w_layout_data*) griddata)->size = sizeof(w_layout_grid_data);
	griddata->flags = W_GRID_VCENTER;
	griddata->widthHint = W_DEFAULT;
	griddata->heightHint = W_DEFAULT;
	griddata->horizontalIndent = 0;
	griddata->verticalIndent = 0;
	griddata->horizontalSpan = 1;
	griddata->verticalSpan = 1;
	griddata->minimumHeight = 0;
	griddata->minimumWidth = 0;
	griddata->cacheWidth = -1;
	griddata->cacheHeight = -1;
	griddata->defaultWhint = 0;
	griddata->defaultHhint = 0;
	griddata->defaultWidth = -1;
	griddata->defaultHeight = -1;
	griddata->currentWhint = 0;
	griddata->currentHhint = 0;
	griddata->currentWidth = -1;
	griddata->currentHeight = -1;
}
void w_layout_grid_init(w_layout_grid *layout, int numColumns,
		int makeColumnsEqualWidth) {
	layout->layout.clazz = &w_layout_grid_class;
	layout->numColumns = numColumns;
	layout->makeColumnsEqualWidth = makeColumnsEqualWidth;
	layout->marginWidth = 5;
	layout->marginHeight = 5;
	layout->marginLeft = 0;
	layout->marginRight = 0;
	layout->marginTop = 0;
	layout->marginBottom = 0;
	layout->horizontalSpacing = 5;
	layout->verticalSpacing = 5;
}
void w_layout_grid_compute_size(w_layout *layout, w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache) {
	w_rect r;
	r.x = 0;
	r.y = 0;
	r.width = wHint;
	r.width = hHint;
	w_layout_grid_do_layout_0((w_layout_grid*) layout, composite, W_FALSE, &r,
			size, flushCache);
	if (wHint != W_DEFAULT)
		size->width = wHint;
	if (hHint != W_DEFAULT)
		size->height = hHint;
	return;
}
void w_layout_grid_do_layout(w_layout *layout, w_composite *composite,
		int flushCache) {
	w_rect r;
	w_size sz;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &r);
	w_layout_grid_do_layout_0((w_layout_grid*) layout, composite, W_TRUE, &r,
			&sz, flushCache);
}
void w_layout_grid_set_data(w_layout *layout, w_control *control,
		const w_layout_data *_data) {
	w_layout_grid_data *data = (w_layout_grid_data*) _data;
	w_layout_stream stream;
	wuchar tmp[sizeof(w_layout_grid_data)];
	stream.sz = 0;
	stream.flags = 0;
	stream.tmp = tmp;
	w_layout_stream_write_int(&stream, 0, data->widthHint + 1);
	w_layout_stream_write_int(&stream, 2, data->heightHint + 1);
	w_layout_stream_write_int(&stream, 4, data->horizontalIndent);
	w_layout_stream_write_int(&stream, 6, data->verticalIndent);
	w_layout_stream_write_int(&stream, 8, data->horizontalSpan - 1);
	w_layout_stream_write_int(&stream, 10, data->verticalSpan - 1);
	w_layout_stream_write_int(&stream, 12, data->minimumWidth);
	w_layout_stream_write_int(&stream, 14, data->minimumHeight);
	/*
	 * caches
	 */
	w_layout_stream_write_int(&stream, 16, data->cacheWidth + 1);
	w_layout_stream_write_int(&stream, 18, data->cacheHeight + 1);
	w_layout_stream_write_int(&stream, 20, data->defaultWhint);
	w_layout_stream_write_int(&stream, 22, data->defaultHhint);
	w_layout_stream_write_int(&stream, 24, data->defaultWidth + 1);
	w_layout_stream_write_int(&stream, 26, data->defaultHeight + 1);
	w_layout_stream_write_int(&stream, 28, data->currentWhint);
	w_layout_stream_write_int(&stream, 30, data->currentHhint);
	w_layout_stream_write_int(&stream, 32, data->currentWidth + 1);
	w_layout_stream_write_int(&stream, 34, data->currentHeight + 1);
	wuint64 flags = data->flags;
	stream.flags |= (flags << 36);
	w_layout_sdata_set(control, &stream, 6);
}
void w_layout_grid_get_data(w_layout *layout, w_control *control,
		w_layout_data *_data) {
	w_layout_grid_data *data = (w_layout_grid_data*) _data;
	w_layout_stream stream;
	w_layout_sdata *d = w_layout_sdata_get(control, &stream, 6);
	if (d != 0) {
		data->flags = stream.flags >> 36;
		data->data.size = d->sz;
		data->data.sign = d->id;
		data->widthHint = w_layout_stream_read_int(&stream, 0) - 1;
		data->heightHint = w_layout_stream_read_int(&stream, 2) - 1;
		data->horizontalIndent = w_layout_stream_read_int(&stream, 4);
		data->verticalIndent = w_layout_stream_read_int(&stream, 6);
		data->horizontalSpan = w_layout_stream_read_int(&stream, 8) + 1;
		data->verticalSpan = w_layout_stream_read_int(&stream, 10) + 1;
		data->minimumWidth = w_layout_stream_read_int(&stream, 12);
		data->minimumHeight = w_layout_stream_read_int(&stream, 14);
		/*
		 * caches
		 */
		data->cacheWidth = w_layout_stream_read_int(&stream, 16) - 1;
		data->cacheHeight = w_layout_stream_read_int(&stream, 18) - 1;
		data->defaultWhint = w_layout_stream_read_int(&stream, 20);
		data->defaultHhint = w_layout_stream_read_int(&stream, 22);
		data->defaultWidth = w_layout_stream_read_int(&stream, 24) - 1;
		data->defaultHeight = w_layout_stream_read_int(&stream, 26) - 1;
		data->currentWhint = w_layout_stream_read_int(&stream, 28);
		data->currentHhint = w_layout_stream_read_int(&stream, 30);
		data->currentWidth = w_layout_stream_read_int(&stream, 32) - 1;
		data->currentHeight = w_layout_stream_read_int(&stream, 34) - 1;
	} else {
		w_layout_grid_data_init(data);
	}
}
/*
 * private function
 */
typedef struct w_grid_row {
	int heights;
	int minHeights;
	int expandRow;
	int vSpan;
} w_grid_row;
typedef struct w_grid_control {
	w_control *control;
	w_layout_grid_data data;
} w_grid_control;
typedef struct w_grid_column {
	int widths;
	int minWidths;
	int expandColumn;
	w_grid_control *currentControl;
	int vSpan;
} w_grid_column;

typedef struct w_grid {
	int children_count;
	int columnCount;
	int rowCount;
	int rowAlloc;
	int currentColumn;
	int currentRow;
	int currentControl;
	w_grid_column *columns;
	w_grid_row *rows;
	w_grid_control children[0];
} w_grid;
w_grid* w_grid_alloc(int children_count, int columnCount) {
	const size_t children_size = children_count * sizeof(w_grid_control);
	const size_t column_size = columnCount * sizeof(w_grid_column);
	const size_t rows_size = children_count * sizeof(w_grid_row);
	const size_t grid_size = sizeof(w_grid);
	const size_t total_size = grid_size + children_size + column_size
			+ rows_size;
	w_grid *grid = (w_grid*) malloc(total_size);
	if (grid == 0)
		return 0;
	grid->columns = (w_grid_column*) ((char*) grid + grid_size + children_size);
	grid->rows = (w_grid_row*) ((char*) grid + grid_size + children_size
			+ column_size);
	memset(grid->columns, 0, column_size + rows_size);
	//memset(grid->rows,0,rows_size);
	return grid;
}
void w_grid_free(w_grid *info) {
	free(info);
}
w_grid* w_grid_load(w_layout_grid *layout, w_composite *composite) {
	w_iterator it;
	w_control *control;
	int count, last_count = 0;
	w_iterator_init(&it);
	w_composite_get_children(composite, &it);
	last_count = w_iterator_get_count(&it);
	if (last_count == 0)
		return 0;
	w_grid *grid = w_grid_alloc(last_count, layout->numColumns);
	if (grid == 0)
		return 0;
	count = 0;
	while (w_iterator_next(&it, &control)) {
		if (count < last_count) {
			w_grid_control *child = &grid->children[count];
			child->control = control;
			w_layout_grid_get_data((w_layout*) layout, child->control,
					(w_layout_data*) &child->data);
			if (child->data.exclude) {
				count--;
			}
		}
		count++;
	}
	grid->children_count = count;
	grid->columnCount = layout->numColumns;
	w_iterator_close(&it);
	return grid;
}
void w_grid_data_compute_size(w_grid_control *control, int wHint, int hHint,
		int flushCache) {
	w_size size;
	if (control->data.cacheWidth != -1 && control->data.cacheHeight != -1)
		return;
	if (wHint == control->data.widthHint && hHint == control->data.heightHint) {
		if (control->data.defaultWidth == -1
				|| control->data.defaultHeight == -1
				|| wHint != control->data.defaultWhint
				|| hHint != control->data.defaultHhint) {
			w_control_compute_size(control->control, &size, wHint, hHint);
			control->data.defaultWhint = wHint;
			control->data.defaultHhint = hHint;
			control->data.defaultWidth = size.width;
			control->data.defaultHeight = size.height;
		}
		control->data.cacheWidth = control->data.defaultWidth;
		control->data.cacheHeight = control->data.defaultHeight;
		return;
	}
	if (control->data.currentWidth == -1 || control->data.currentHeight == -1
			|| wHint != control->data.currentWhint
			|| hHint != control->data.currentHhint) {
		w_control_compute_size(control->control, &size, wHint, hHint);
		control->data.currentWhint = wHint;
		control->data.currentHhint = hHint;
		control->data.currentWidth = size.width;
		control->data.currentHeight = size.height;
	}
	control->data.cacheWidth = control->data.currentWidth;
	control->data.cacheHeight = control->data.currentHeight;
}
void w_grid_compute_sizes(w_grid *grid, int flushCache) {
	size_t children_count = grid->children_count;
	for (size_t i = 0; i < children_count; i++) {
		w_grid_control *child = &grid->children[i];
		//if (flushCache) data.flushCache ();
		w_grid_data_compute_size(child, child->data.widthHint,
				child->data.heightHint, flushCache);
		if (child->data.grabExcessHorizontalSpace
				&& child->data.minimumWidth > 0) {
			if (child->data.cacheWidth < child->data.minimumWidth) {
				int trim = 0;
				w_rect r, res;
				//TEMPORARY CODE
				if (w_widget_class_id(W_WIDGET(child->control))
						>= _W_CLASS_SCROLLABLE) {
					memset(&r, 0, sizeof(r));
					w_scrollable_compute_trim(W_SCROLLABLE(child->control),
							&res, &r);
					trim = res.width;
				} else {
					trim = w_control_get_border_width(child->control) * 2;
				}
				child->data.cacheWidth = child->data.cacheHeight = W_DEFAULT;
				w_grid_data_compute_size(child,
						WMAX(0, (child->data.minimumWidth - trim)),
						child->data.heightHint, flushCache);
			}
		}
		if (child->data.grabExcessVerticalSpace
				&& child->data.minimumHeight > 0) {
			child->data.cacheHeight = WMAX(child->data.cacheHeight,
					child->data.minimumHeight);
		}
	}
}
void w_grid_row_reset(w_grid *grid) {
	w_grid_column *columns = grid->columns;
	size_t columnCount = grid->columnCount;
	grid->currentColumn = 0;
	grid->currentControl = 0;
	grid->currentRow = 0;
	for (size_t i = 0; i < columnCount; i++) {
		columns[i].vSpan = 0;
		columns[i].currentControl = 0;
	}
}
int w_grid_row_next(w_grid *grid, w_grid_row **row) {
	grid->currentColumn = 0;
	if (grid->currentControl >= grid->children_count)
		return -1;
	*row = &grid->rows[grid->currentRow];
	w_grid_column *columns = grid->columns;
	size_t columnCount = grid->columnCount;
	if (grid->currentRow > 0) {
		size_t last_vSpan = grid->rows[grid->currentRow - 1].vSpan;
		for (size_t i = 0; i < columnCount; i++) {
			columns[i].vSpan -= last_vSpan;
		}
	}
	for (size_t i = 0; i < columnCount; i++) {
		if (grid->currentControl < grid->children_count) {
			w_grid_control *c = &grid->children[grid->currentControl];
			size_t _hspan_min = WMIN(c->data.horizontalSpan, columnCount);
			size_t hSpan = WMAX(1, _hspan_min);
			size_t vSpan = WMAX(1, c->data.verticalSpan);
			size_t column = i;
			while (column < columnCount && (column - i) < hSpan
					&& columns[column].vSpan <= 0) {
				column++;
			}
			if ((column - i) >= hSpan) {
				for (size_t j = i; j < column; j++) {
					columns[j].vSpan = vSpan;
					columns[j].currentControl = c;
				}
				i = column - 1;
				grid->currentControl++;
			}
		}
	}
	size_t minvSpan = -1;
	for (size_t i = 0; i < columnCount; i++) {
		if (columns[i].vSpan > 0) {
			if (minvSpan > ((size_t) columns[i].vSpan)) {
				minvSpan = columns[i].vSpan;
			}
		}
	}
	(*row)->vSpan = minvSpan;
	return grid->currentRow++;
}
void w_grid_column_reset(w_grid *grid) {
	grid->currentColumn = 0;
}
int w_grid_column_next(w_grid *grid, w_grid_column **column,
		w_grid_control **control, int first) {
	if (grid->currentColumn < grid->columnCount) {
		w_grid_column *col = &grid->columns[grid->currentColumn];
		w_grid_control *c = col->currentControl;
		int hSpan_min = WMIN(c->data.horizontalSpan, grid->columnCount);
		int hSpan = WMAX(1, hSpan_min);
		int vSpan = WMAX(1, c->data.verticalSpan);
		if (first) {
			int j = grid->currentColumn + hSpan - 1;
			if (col->vSpan == vSpan && 0 <= j && j < grid->columnCount) {
			} else {
				c = 0;
			}
		} else {
			int j = grid->currentColumn - hSpan + 1;
			if (col->vSpan == 1 && 0 <= j && j < grid->columnCount) {
			} else {
				c = 0;
			}
		}
		*control = c;
		*column = col;
		return grid->currentColumn++;
	} else
		return -1;
}
void w_grid_column_with(w_grid *grid, w_layout_grid *layout, w_rect *r) {
	int availableWidth = r->width
			- layout->horizontalSpacing * (grid->columnCount - 1)
			- (layout->marginLeft + layout->marginWidth * 2
					+ layout->marginRight);
	int expandCount = 0;
	int columnCount = grid->columnCount;
	int i, j;
	w_grid_row *row;
	w_grid_column *column, *columns = grid->columns;
	w_grid_control *child;
	grid->rowCount = 0;
	w_grid_row_reset(grid);
	while ((i = w_grid_row_next(grid, &row)) >= 0) {
		while ((j = w_grid_column_next(grid, &column, &child, W_TRUE)) >= 0) {
			if (child != 0) {
				int hSpan_min = WMIN(child->data.horizontalSpan, columnCount);
				int hSpan = WMAX(1, hSpan_min);
				if (hSpan == 1) {
					int w = child->data.cacheWidth
							+ child->data.horizontalIndent;
					column->widths = WMAX(column->widths, w);
					if (child->data.grabExcessHorizontalSpace) {
						if (!column->expandColumn)
							expandCount++;
						column->expandColumn = W_TRUE;
					}
					if (!child->data.grabExcessHorizontalSpace
							|| child->data.minimumWidth != 0) {
						w = !child->data.grabExcessHorizontalSpace
								|| child->data.minimumWidth == W_DEFAULT ?
								child->data.cacheWidth :
								child->data.minimumWidth;
						w += child->data.horizontalIndent;
						column->minWidths = WMAX(column->minWidths, w);
					}
				}
			}
		}
		grid->rowCount += row->vSpan;
		grid->rowAlloc = i;
	}
	grid->rowAlloc += 1;
	w_grid_row_reset(grid);
	while ((i = w_grid_row_next(grid, &row)) >= 0) {
		while ((j = w_grid_column_next(grid, &column, &child, W_FALSE)) >= 0) {
			if (child != 0) {
				int hSpan = WMAX(1,
						WMIN(child->data.horizontalSpan, columnCount));
				if (hSpan > 1) {
					int spanWidth = 0, spanMinWidth = 0, spanExpandCount = 0;
					for (int k = 0; k < hSpan; k++) {
						spanWidth += columns[j - k].widths;
						spanMinWidth += columns[j - k].minWidths;
						if (columns[j - k].expandColumn)
							spanExpandCount++;
					}
					if (child->data.grabExcessHorizontalSpace
							&& spanExpandCount == 0) {
						expandCount++;
						columns[j].expandColumn = W_TRUE;
					}
					int w = child->data.cacheWidth
							+ child->data.horizontalIndent - spanWidth
							- (hSpan - 1) * layout->horizontalSpacing;
					if (w > 0) {
						if (layout->makeColumnsEqualWidth) {
							int equalWidth = (w + spanWidth) / hSpan;
							int remainder = (w + spanWidth) % hSpan, last = -1;
							for (int k = 0; k < hSpan; k++) {
								columns[last = j - k].widths = WMAX(equalWidth,
										columns[j - k].widths);
							}
							if (last > -1)
								columns[last].widths += remainder;
						} else {
							if (spanExpandCount == 0) {
								columns[j].widths += w;
							} else {
								int delta = w / spanExpandCount;
								int remainder = w % spanExpandCount, last = -1;
								for (int k = 0; k < hSpan; k++) {
									if (columns[j - k].expandColumn) {
										columns[last = j - k].widths += delta;
									}
								}
								if (last > -1)
									columns[last].widths += remainder;
							}
						}
					}
					if (!child->data.grabExcessHorizontalSpace
							|| child->data.minimumWidth != 0) {
						w = !child->data.grabExcessHorizontalSpace
								|| child->data.minimumWidth == W_DEFAULT ?
								child->data.cacheWidth :
								child->data.minimumWidth;
						w += child->data.horizontalIndent - spanMinWidth
								- (hSpan - 1) * layout->horizontalSpacing;
						if (w > 0) {
							if (spanExpandCount == 0) {
								columns[j].minWidths += w;
							} else {
								int delta = w / spanExpandCount;
								int remainder = w % spanExpandCount, last = -1;
								for (int k = 0; k < hSpan; k++) {
									if (columns[j - k].expandColumn) {
										columns[last = j - k].minWidths +=
												delta;
									}
								}
								if (last > -1)
									columns[last].minWidths += remainder;
							}
						}
					}
				}
			}
		}
	}
	if (layout->makeColumnsEqualWidth) {
		int minColumnWidth = 0;
		int columnWidth = 0;
		for (int i = 0; i < columnCount; i++) {
			minColumnWidth = WMAX(minColumnWidth, columns[i].minWidths);
			columnWidth = WMAX(columnWidth, columns[i].widths);
		}
		columnWidth =
				r->width == W_DEFAULT || expandCount == 0 ?
						columnWidth :
						WMAX(minColumnWidth, availableWidth / columnCount);
		for (int i = 0; i < columnCount; i++) {
			columns[i].expandColumn = expandCount > 0;
			columns[i].widths = columnWidth;
		}
	} else {
		if (r->width != W_DEFAULT && expandCount > 0) {
			int totalWidth = 0;
			for (int i = 0; i < columnCount; i++) {
				totalWidth += columns[i].widths;
			}
			int c = expandCount;
			int delta = (availableWidth - totalWidth) / c;
			int remainder = (availableWidth - totalWidth) % c;
			int last = -1;
			while (totalWidth != availableWidth) {
				for (int j = 0; j < columnCount; j++) {
					if (columns[j].expandColumn) {
						if (columns[j].widths + delta > columns[j].minWidths) {
							columns[last = j].widths = columns[j].widths
									+ delta;
						} else {
							columns[j].widths = columns[j].minWidths;
							columns[j].expandColumn = W_FALSE;
							c--;
						}
					}
				}
				if (last > -1)
					columns[last].widths += remainder;

				w_grid_row_reset(grid);
				while ((i = w_grid_row_next(grid, &row)) >= 0) {
					while ((j = w_grid_column_next(grid, &column, &child,
							W_FALSE)) >= 0) {
						if (child != 0) {
							int hSpan =
									WMAX(1,
											WMIN(child->data.horizontalSpan, columnCount));
							if (hSpan > 1) {
								if (!child->data.grabExcessHorizontalSpace
										|| child->data.minimumWidth != 0) {
									int spanWidth = 0, spanExpandCount = 0;
									for (int k = 0; k < hSpan; k++) {
										spanWidth += columns[j - k].widths;
										if (columns[j - k].expandColumn)
											spanExpandCount++;
									}
									int w =
											!child->data.grabExcessHorizontalSpace
													|| child->data.minimumWidth
															== W_DEFAULT ?
													child->data.cacheWidth :
													child->data.minimumWidth;
									w += child->data.horizontalIndent
											- spanWidth
											- (hSpan - 1)
													* layout->horizontalSpacing;
									if (w > 0) {
										if (spanExpandCount == 0) {
											columns[j].widths += w;
										} else {
											int delta2 = w / spanExpandCount;
											int remainder2 = w
													% spanExpandCount, last2 =
													-1;
											for (int k = 0; k < hSpan; k++) {
												if (columns[j - k].expandColumn) {
													columns[last2 = j - k].widths +=
															delta2;
												}
											}
											if (last2 > -1)
												columns[last2].widths +=
														remainder2;
										}
									}
								}
							}
						}
					}
				}
				if (c == 0)
					break;
				totalWidth = 0;
				for (int i = 0; i < columnCount; i++) {
					totalWidth += columns[i].widths;
				}
				delta = (availableWidth - totalWidth) / c;
				remainder = (availableWidth - totalWidth) % c;
				last = -1;
			}
		}
	}
}
void w_grid_wrapping(w_grid *grid, w_layout_grid *layout, w_rect *r) {
	if (r->width != W_DEFAULT) {
		int columnCount = grid->columnCount;
		w_grid_row *row;
		w_grid_column *column;
		w_grid_control *child;
		int i, j;
		w_grid_row_reset(grid);
		while ((i = w_grid_row_next(grid, &row)) >= 0) {
			while ((j = w_grid_column_next(grid, &column, &child, W_FALSE)) >= 0) {
				if (child != 0) {
					if (child->data.heightHint == W_DEFAULT) {
						//TEMPORARY CODE
						int hSpan = WMAX(1,
								WMIN(child->data.horizontalSpan, columnCount));
						int currentWidth = 0;
						for (int k = 0; k < hSpan; k++) {
							currentWidth += grid->columns[j - k].widths;
						}
						currentWidth += (hSpan - 1) * layout->horizontalSpacing
								- child->data.horizontalIndent;
						if ((currentWidth != child->data.cacheWidth
								&& child->data.horizontalAlignment
										== W_GRID_FILL)
								|| (child->data.cacheWidth > currentWidth)) {
							int trim = 0;
							if (w_widget_class_id(W_WIDGET(child->control))
									>= _W_CLASS_SCROLLABLE) {
								w_rect rect, res;
								memset(&r, 0, sizeof(r));
								w_scrollable_compute_trim(
										W_SCROLLABLE(child->control), &res,
										&rect);
								trim = rect.width;
							} else {
								trim = w_control_get_border_width(
										child->control) * 2;
							}
							child->data.cacheWidth = child->data.cacheHeight =
									W_DEFAULT;
							w_grid_data_compute_size(child,
									WMAX(0, currentWidth - trim),
									child->data.heightHint, W_FALSE);
							if (child->data.grabExcessVerticalSpace
									&& child->data.minimumHeight > 0) {
								child->data.cacheHeight = WMAX(
										child->data.cacheHeight,
										child->data.minimumHeight);
							}
							/*if (flush == null) flush = new GridData [count];
							 flush [flushLength++] = data;*/
						}
					}
				}
			}
		}
	}
}
void w_grid_row_heights(w_grid *grid, w_layout_grid *layout, w_rect *r) {
	int availableHeight = r->height
			- layout->verticalSpacing * (grid->rowCount - 1)
			- (layout->marginTop + layout->marginHeight * 2
					+ layout->marginBottom);
	int expandCount = 0;
	int columnCount = grid->columnCount;
	w_grid_row *row, *rows = grid->rows;
	w_grid_column *column;
	w_grid_control *child;
	int i, j;
	w_grid_row_reset(grid);
	while ((i = w_grid_row_next(grid, &row)) >= 0) {
		while ((j = w_grid_column_next(grid, &column, &child, W_TRUE)) >= 0) {
			if (child != 0) {
				int vSpan = WMAX(1,
						WMIN(child->data.verticalSpan, grid->rowCount));
				if (vSpan == 1) {
					int h = child->data.cacheHeight
							+ child->data.verticalIndent;
					row->heights = WMAX(row->heights, h);
					if (child->data.grabExcessVerticalSpace) {
						if (!row->expandRow)
							expandCount++;
						row->expandRow = W_TRUE;
					}
					if (!child->data.grabExcessVerticalSpace
							|| child->data.minimumHeight != 0) {
						h = !child->data.grabExcessVerticalSpace
								|| child->data.minimumHeight == W_DEFAULT ?
								child->data.cacheHeight :
								child->data.minimumHeight;
						h += child->data.verticalIndent;
						row->minHeights = WMAX(row->minHeights, h);
					}
				}
			}
		}
		w_grid_column_reset(grid);
		while ((j = w_grid_column_next(grid, &column, &child, W_FALSE)) >= 0) {
			if (child != 0) {
				int vSpan = WMAX(1,
						WMIN(child->data.verticalSpan, grid->rowCount));
				if (vSpan > 1) {
					int spanHeight = 0, spanMinHeight = 0, spanExpandCount = 0;
					for (int k = 0, l = 0; k < vSpan; l++) {
						spanHeight += rows[i - l].heights;
						spanMinHeight += rows[i - l].minHeights;
						if (rows[i - l].expandRow)
							spanExpandCount++;
						k += rows[i - l].vSpan;
					}
					if (child->data.grabExcessVerticalSpace
							&& spanExpandCount == 0) {
						expandCount++;
						rows[i].expandRow = W_TRUE;
					}
					int h = child->data.cacheHeight + child->data.verticalIndent
							- spanHeight
							- (vSpan - 1) * layout->verticalSpacing;
					if (h > 0) {
						if (spanExpandCount == 0) {
							rows[i].heights += h;
						} else {
							int delta = h / spanExpandCount;
							int remainder = h % spanExpandCount, last = -1;
							for (int k = 0; k < vSpan;) {
								if (rows[i - k].expandRow) {
									rows[last = i - k].heights += delta;
								}
								k += rows[i - k].vSpan;
							}
							if (last > -1)
								rows[last].heights += remainder;
						}
					}
					if (!child->data.grabExcessVerticalSpace
							|| child->data.minimumHeight != 0) {
						h = !child->data.grabExcessVerticalSpace
								|| child->data.minimumHeight == W_DEFAULT ?
								child->data.cacheHeight :
								child->data.minimumHeight;
						h += child->data.verticalIndent - spanMinHeight
								- (vSpan - 1) * layout->verticalSpacing;
						if (h > 0) {
							if (spanExpandCount == 0) {
								rows[i].minHeights += h;
							} else {
								int delta = h / spanExpandCount;
								int remainder = h % spanExpandCount, last = -1;
								for (int k = 0, l = 0; k < vSpan; l++) {
									if (rows[i - l].expandRow) {
										rows[last = i - l].minHeights += delta;
									}
									k += rows[i - l].vSpan;
								}
								if (last > -1)
									rows[last].minHeights += remainder;
							}
						}
					}
				}
			}
		}
	}
	if (r->height != W_DEFAULT && expandCount > 0) {
		int totalHeight = 0;
		size_t rowCount = grid->rowAlloc;
		for (int i = 0; i < rowCount; i++) {
			totalHeight += rows[i].heights;
		}
		int c = expandCount;
		int delta = (availableHeight - totalHeight) / c;
		int remainder = (availableHeight - totalHeight) % c;
		int last = -1;
		while (totalHeight != availableHeight) {
			for (int i = 0; i < rowCount; i++) {
				if (rows[i].expandRow) {
					if (rows[i].heights + delta > rows[i].minHeights) {
						rows[last = i].heights = rows[i].heights + delta;
					} else {
						rows[i].heights = rows[i].minHeights;
						rows[i].expandRow = W_FALSE;
						c--;
					}
				}
			}
			if (last > -1)
				rows[last].heights += remainder;

			w_grid_row_reset(grid);
			while ((i = w_grid_row_next(grid, &row)) >= 0) {
				while ((j = w_grid_column_next(grid, &column, &child, W_FALSE))
						>= 0) {
					if (child != 0) {
						int vSpan = WMAX(1,
								WMIN(child->data.verticalSpan, grid->rowCount));
						if (vSpan > 1) {
							if (!child->data.grabExcessVerticalSpace
									|| child->data.minimumHeight != 0) {
								int spanHeight = 0, spanExpandCount = 0;
								for (int k = 0, l = 0; k < vSpan; l++) {
									spanHeight += rows[i - l].heights;
									if (rows[i - l].expandRow)
										spanExpandCount++;
									k += rows[i - l].vSpan;
								}
								int h = !child->data.grabExcessVerticalSpace
										|| child->data.minimumHeight
												== W_DEFAULT ?
										child->data.cacheHeight :
										child->data.minimumHeight;
								h += child->data.verticalIndent - spanHeight
										- (vSpan - 1) * layout->verticalSpacing;
								if (h > 0) {
									if (spanExpandCount == 0) {
										rows[i].heights += h;
									} else {
										int delta2 = h / spanExpandCount;
										int remainder2 = h % spanExpandCount,
												last2 = -1;
										for (int k = 0, l = 0; k < vSpan; l++) {
											if (rows[i - l].expandRow) {
												rows[last2 = i - l].heights +=
														delta2;
											}
											k += rows[i - l].vSpan;
										}
										if (last2 > -1)
											rows[last2].heights += remainder2;
									}
								}
							}
						}
					}
				}
			}
			if (c == 0)
				break;
			totalHeight = 0;
			for (int i = 0; i < rowCount; i++) {
				totalHeight += rows[i].heights;
			}
			delta = (availableHeight - totalHeight) / c;
			remainder = (availableHeight - totalHeight) % c;
			last = -1;
		}
	}

}
void w_grid_move_control(w_grid *grid, w_layout_grid *layout, w_rect *r) {
	int gridY = r->y + layout->marginTop + layout->marginHeight;
	int rowCount = grid->rowCount;
	int columnCount = grid->columnCount;
	int i, j;
	w_grid_row *row;
	w_grid_column *column;
	w_grid_control *child;
	w_grid_row_reset(grid);
	while ((i = w_grid_row_next(grid, &row)) >= 0) {
		int gridX = r->x + layout->marginLeft + layout->marginWidth;
		while ((j = w_grid_column_next(grid, &column, &child, W_TRUE)) >= 0) {
			if (child != 0) {
				int hSpan = WMAX(1,
						WMIN(child->data.horizontalSpan, columnCount));
				int vSpan = WMAX(1, child->data.verticalSpan);
				int cellWidth = 0, cellHeight = 0;
				for (int k = 0; k < hSpan; k++) {
					cellWidth += grid->columns[j + k].widths;
				}
				for (int k = 0, l = 0; k < vSpan; l++) {
					if ((i + l) > rowCount)
						break;
					cellHeight += grid->rows[i + l].heights;
					k += grid->rows[i + l].vSpan;
				}
				cellWidth += layout->horizontalSpacing * (hSpan - 1);
				int childX = gridX + child->data.horizontalIndent;
				int childWidth = WMIN(child->data.cacheWidth, cellWidth);
				switch (child->data.horizontalAlignment) {
				case W_GRID_CENTER:
					childX += WMAX(0,
							(cellWidth - child->data.horizontalIndent
									- childWidth) / 2);
					break;
				case W_GRID_END:
					childX += WMAX(0,
							cellWidth - child->data.horizontalIndent
									- childWidth);
					break;
				case W_GRID_FILL:
					childWidth = cellWidth - child->data.horizontalIndent;
					break;
				}
				cellHeight += layout->verticalSpacing * (vSpan - 1);
				int childY = gridY + child->data.verticalIndent;
				int childHeight = WMIN(child->data.cacheHeight, cellHeight);
				switch (child->data.verticalAlignment) {
				case W_GRID_CENTER:
					childY += WMAX(0,
							(cellHeight - child->data.verticalIndent
									- childHeight) / 2);
					break;
				case W_GRID_END:
					childY += WMAX(0,
							cellHeight - child->data.verticalIndent
									- childHeight);
					break;
				case W_GRID_FILL:
					childHeight = cellHeight - child->data.verticalIndent;
					break;
				}
				w_grid_control *c = child;
				if (c != 0) {
					w_rect rect;
					rect.x = childX;
					rect.y = childY;
					rect.width = childWidth;
					rect.height = childHeight;
					w_control_set_bounds(c->control, &rect.pt, &rect.sz);
				}
			}
			gridX += grid->columns[j].widths + layout->horizontalSpacing;
		}
		gridY += grid->rows[i].heights + layout->verticalSpacing;
	}
}
void w_layout_grid_do_layout_0(w_layout_grid *layout, w_composite *composite,
		int move, w_rect *r, w_size *sz, int flushCache) {
	w_grid *grid = 0;
	if (layout->numColumns >= 1) {
		grid = w_grid_load(layout, composite);
		if (grid != 0) {
			w_grid_compute_sizes(grid, W_FALSE);
			w_grid_column_with(grid, layout, r);
			w_grid_wrapping(grid, layout, r);
			w_grid_row_heights(grid, layout, r);
			if (move)
				w_grid_move_control(grid, layout, r);
			sz->width = 0;
			sz->height = 0;
			for (int i = 0; i < grid->columnCount; i++) {
				sz->width += grid->columns[i].widths;
			}
			for (int i = 0; i < grid->rowAlloc; i++) {
				sz->height += grid->rows[i].heights;
			}
			sz->width += layout->horizontalSpacing * (grid->columnCount - 1)
					+ layout->marginLeft + layout->marginWidth * 2
					+ layout->marginRight;
			sz->height += layout->verticalSpacing * (grid->rowCount - 1)
					+ layout->marginTop + layout->marginHeight * 2
					+ layout->marginBottom;
			w_grid_free(grid);
			return;
		}
	}
	sz->width = layout->marginLeft + layout->marginWidth * 2
			+ layout->marginRight;
	sz->height = layout->marginTop + layout->marginHeight * 2
			+ layout->marginBottom;
	return;
}
_w_layout_class w_layout_grid_class = { //
		w_layout_grid_compute_size, //
				w_layout_flush_cache0, //
				w_layout_grid_do_layout, //
				w_layout_grid_set_data, //
				w_layout_grid_get_data //
		};
