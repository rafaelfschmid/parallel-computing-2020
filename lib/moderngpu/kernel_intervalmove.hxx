// moderngpu copyright (c) 2016, Sean Baxter http://www.moderngpu.com
#pragma once
#include "kernel_load_balance.hxx"

BEGIN_MGPU_NAMESPACE

template<typename launch_arg_t = empty_t, typename input_it, 
  typename segments_it, typename output_it>
void interval_expand(input_it input, int count, segments_it segments,
  int num_segments, output_it output, context_t& context) {

  typedef typename std::iterator_traits<input_it>::value_type type_t;
  transform_lbs<launch_arg_t>(
    [=]MGPU_DEVICE(int index, int seg, int rank, tuple<type_t> desc) {
      output[index] = get<0>(desc);
    }, 
    count, segments, num_segments, make_tuple(input), context
  );
}

template<typename launch_arg_t = empty_t, typename input_it, 
  typename segments_it, typename gather_it, typename output_it>
void interval_gather(input_it input, int count, segments_it segments,
  int num_segments, gather_it gather, output_it output, context_t& context) {

  transform_lbs<launch_arg_t>(
    [=]MGPU_DEVICE(int index, int seg, int rank, tuple<int> desc) {
      output[index] = input[get<0>(desc) + rank];
    }, 
    count, segments, num_segments, make_tuple(gather), context
  );
}

template<typename launch_arg_t = empty_t, typename input_it, 
  typename segments_it, typename scatter_it, typename output_it>
void interval_scatter(input_it input, int count, segments_it segments,
  int num_segments, scatter_it scatter, output_it output, context_t& context) {

  transform_lbs<launch_arg_t>(
    [=]MGPU_DEVICE(int index, int seg, int rank, tuple<int> desc) {
      output[get<0>(desc) + rank] = input[index];
    }, 
    count, segments, num_segments, make_tuple(scatter), context
  );
}

template<typename launch_arg_t = empty_t, 
  typename input_it, typename segments_it, typename scatter_it,
  typename gather_it, typename output_it>
void interval_move(input_it input, int count, segments_it segments,
  int num_segments, scatter_it scatter, gather_it gather, output_it output, 
  context_t& context) {

  transform_lbs<launch_arg_t>(
    [=]MGPU_DEVICE(int index, int seg, int rank, tuple<int, int> desc) {
      output[get<0>(desc) + rank] = input[get<1>(desc) + rank];
    }, 
    count, segments, num_segments, make_tuple(scatter, gather), context
  );
}

END_MGPU_NAMESPACE
