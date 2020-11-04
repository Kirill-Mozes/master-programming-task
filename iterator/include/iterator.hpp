/* Image iterator.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <algorithm>
#include <type_traits>
#include <vector>
#include <utility>

#include <boost/iterator/iterator_adaptor.hpp>

//{ image iterator
template<class Iterator>
class image_iterator: public boost::iterator_adaptor<image_iterator<Iterator>, Iterator>
{
    using ItAdaptorType = boost::iterator_adaptor<image_iterator<Iterator>, Iterator>;
public:
    image_iterator(Iterator inp_iter, size_t wid, size_t str) : ItAdaptorType(inp_iter),  
        stride(str),
        width(wid) 
    {
    }
private:
    friend class boost::iterator_core_access;
    typename ItAdaptorType::reference dereference() const
    {
        return *(this->base_reference());
    }

    void increment()
    {
        ++crnt_pos;
        if (crnt_pos >= ptrdiff_t(width))
        {
            crnt_pos = 0;
            this->base_reference() += stride - width + 1;
        }
        else
        {
            this->base_reference()++;
        }
    }

    void decrement()
    {
        --crnt_pos;
        if (crnt_pos < 0)
        {
            crnt_pos = width - 1;
            this->base_reference() -= (stride - width + 1);
        }
        else
        {
            this->base_reference()--;
        }
    }

    void advance(typename ItAdaptorType::difference_type p)
    {
        ptrdiff_t num_rows = (crnt_pos + p)/ ptrdiff_t(width);
        
        crnt_pos += p;
        if (crnt_pos < 0)
        {
            --num_rows;
        }
        crnt_pos %= width;
        this->base_reference() = this->base_reference() + p + num_rows * ptrdiff_t(stride - width);
    }

    typename ItAdaptorType::difference_type distance_to(
        image_iterator const& y) const
    {
        ptrdiff_t raw_distance = y.base() - this->base() - y.crnt_pos + this->crnt_pos;
        ptrdiff_t num_rows= raw_distance/ptrdiff_t(stride);

        return y.base() - this->base() - num_rows*(stride - width);
    };

    bool equal(ItAdaptorType const& y) const
    {
        return this->base() == y.base();
    }

    size_t stride;
    size_t width;
    ptrdiff_t crnt_pos = 0;
    
   
};
//}


template<class Container = std::vector<uint8_t>>
class image
{
public:
    image(size_t width, size_t height, size_t stride):
        data(stride * height),
        width_(width),
        stride_(stride)
    {}

    auto begin()
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end()
    {
        return image_iterator(data.end(), width_, stride_);
    }

    auto begin() const
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end() const
    {
        return image_iterator(data.end(), width_, stride_);
    }

private:
    Container data; // image data including strides. Access by (x, y): data[y * stride_ + x]
    size_t width_;
    size_t stride_;
};

#endif // __ITERATOR_HPP__
