from __future__ import (division, unicode_literals, print_function, absolute_import)

import abc

from six import add_metaclass
from future import standard_library
standard_library.install_aliases()


@add_metaclass(abc.ABCMeta)
class Data(object):
    """
    The base class of data that can be returned by the pyuda Client.
    """

    @abc.abstractmethod
    def plot(self):
        pass

    @abc.abstractmethod
    def widget(self):
        pass

    def jsonify(self, indent=None):
        raise NotImplementedError("jsonify has not been implement for this data class")