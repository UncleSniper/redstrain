#ifndef REDSTRAIN_MOD_ALGORITHM_SEQUENCEMAP_HPP
#define REDSTRAIN_MOD_ALGORITHM_SEQUENCEMAP_HPP

#include <redstrain/util/Delete.hpp>
#include <redstrain/util/Appender.hpp>
#include <redstrain/util/BareType.hpp>
#include <redstrain/util/WithAlign.hpp>
#include <redstrain/util/MapKeyIterator.hpp>
#include <redstrain/util/types.hpp>
#include <redstrain/util/destructors.hpp>

#include "allocators.hpp"

namespace redengine {
namespace algorithm {

	template<
		typename KeyT,
		typename ValueT,
		typename NameT = void,
		void (*ValueDestructor)(ValueT&) = util::explicitDestructor<ValueT>,
		void* (*Allocator)(util::MemorySize) = standardAlloc,
		void (*Deallocator)(void*) = standardFree
	>
	class SequenceMap {

	  public:
		typedef KeyT Key;
		typedef ValueT Value;
		typedef NameT Name;

	  private:
		typedef typename util::WithAlign<util::AlignOf<ValueT>::ALIGNMENT>::Primitive AlignValue;

	  private:
		util::MemorySize cursize;

	  public:
		template<typename NodeKeyT, typename NodeValueT, typename NodeNameT>
		class NodeBase {

		  public:
			typedef NodeKeyT Key;
			typedef NodeValueT Value;
			typedef NodeNameT Name;

		  protected:
			NodeNameT name;

		  protected:
			NodeBase(const NodeBase& node) : name(node.name) {}

		  public:
			NodeBase() {}

			inline const NodeNameT& getName() const {
				return name;
			}

			inline NodeNameT& getName() {
				return name;
			}

			void setName(const NodeNameT& name) {
				this->name = name;
			}

		};

		template<typename NodeKeyT, typename NodeValueT>
		class NodeBase<NodeKeyT, NodeValueT, void> {

		  public:
			typedef NodeKeyT Key;
			typedef NodeValueT Value;
			typedef void Name;

		  protected:
			NodeBase(const NodeBase&) {}

		  public:
			NodeBase() {}

		};

		class PublicNode : public NodeBase<KeyT, ValueT, NameT> {

		  public:
			template<bool Constant>
			class MaybeNode {

			  private:
				template<int, bool>
				struct NodeType {
					typedef PublicNode Node;
				};

				template<int Dummy>
				struct NodeType<Dummy, true> {
					typedef const PublicNode Node;
				};

			  public:
				typedef typename NodeType<0, Constant>::Node Node;

			  private:
				Node* node;

			  public:
				inline MaybeNode(Node* node = NULL) : node(node) {}
				inline MaybeNode(const MaybeNode& node) : node(node.node) {}

				inline Node* getNode() const {
					return node;
				}

				inline operator bool() const {
					return !!node;
				}

				inline Node* operator->() const {
					return node;
				}

				inline operator Node*() const {
					return node;
				}

				inline operator Node&() const {
					return *node;
				}

				inline MaybeNode get(const KeyT& childKey) const {
					if(!node)
						return NULL;
					return node->get(childKey);
				}

				inline bool remove() const {
					return node ? node->remove() : false;
				}

				bool clear() const {
					if(!node)
						return false;
					node->clear();
					return true;
				}

			};

		  protected:
			typedef std::map<KeyT, PublicNode*> Children;
			typedef typename Children::iterator ChildIterator;
			typedef typename Children::const_iterator ConstChildIterator;

		  public:
			typedef util::MapKeyIterator<KeyT, PublicNode*> KeyIterator;

		  protected:
			SequenceMap& map;
			PublicNode *const parent;
			const KeyT key;
			union {
				char buffer[sizeof(ValueT)];
				AlignValue align;
			} value;
			bool valuePresent;
			Children children;

		  protected:
			PublicNode(const PublicNode& node) : NodeBase<KeyT, ValueT, NameT>(node), map(node.map),
					parent(node.parent), key(node.key), valuePresent(false) {}

		  private:
			void getKeyPathImpl(util::Appender<KeyT>& sink) const {
				if(!parent)
					return;
				parent->getKeyPath(sink);
				sink.append(key);
			}

		  public:
			PublicNode(SequenceMap& map, PublicNode* parent, const KeyT& key, const ValueT& value)
					: map(map), parent(parent), key(key), valuePresent(true) {
				new(this->value.buffer) ValueT(value);
			}

			PublicNode(SequenceMap& map, PublicNode* parent, const KeyT& key)
					: map(map), parent(parent), key(key), valuePresent(false) {}

			~PublicNode() {
				if(valuePresent) {
					ValueDestructor(*reinterpret_cast<ValueT*>(value.buffer));
					--map.cursize;
				}
				ConstChildIterator begin(children.begin()), end(children.end());
				for(; begin != end; ++begin)
					delete begin->second;
			}

			inline SequenceMap& getMap() {
				return map;
			}

			inline const SequenceMap& getMap() const {
				return map;
			}

			MaybeNode<false> getParent() {
				return parent;
			}

			MaybeNode<true> getParent() const {
				return parent;
			}

			inline const KeyT& getKey() const {
				return key;
			}

			inline bool hasValue() const {
				return valuePresent;
			}

			ValueT& getValue() {
				return *reinterpret_cast<ValueT*>(value.buffer);
			}

			const ValueT& getValue() const {
				return *reinterpret_cast<const ValueT*>(value.buffer);
			}

			void setValue(const ValueT& newValue) {
				if(valuePresent) {
					ValueDestructor(*reinterpret_cast<ValueT*>(value.buffer));
					valuePresent = false;
					--map.cursize;
				}
				new(reinterpret_cast<ValueT*>(this->value.buffer)) ValueT(newValue);
				valuePresent = true;
				++map.cursize;
			}

			bool isLeaf() const {
				return children.empty();
			}

			void getKeys(KeyIterator& begin, KeyIterator& end) const {
				begin = KeyIterator(children.begin());
				end = KeyIterator(children.end());
			}

			KeyIterator beginKeys() const {
				return KeyIterator(children.begin());
			}

			KeyIterator endKeys() const {
				return KeyIterator(children.end());
			}

			MaybeNode<false> get(const KeyT& childKey) {
				ConstChildIterator it(children.find(childKey));
				return it == children.end() ? NULL : it->second;
			}

			MaybeNode<true> get(const KeyT& childKey) const {
				ConstChildIterator it(children.find(childKey));
				return it == children.end() ? NULL : it->second;
			}

			PublicNode& put(const KeyT& childKey) {
				ConstChildIterator it(children.find(childKey));
				if(it != children.end())
					return *it->second;
				util::Delete<PublicNode> node(new PublicNode(map, this, childKey));
				children[childKey] = *node;
				return *node.set();
			}

			PublicNode& put(const KeyT& childKey, const ValueT& childValue) {
				ConstChildIterator it(children.find(childKey));
				if(it != children.end()) {
					it->second->setValue(childValue);
					return *it->second;
				}
				util::Delete<PublicNode> node(new PublicNode(map, this, childKey, childValue));
				children[childKey] = *node;
				return *node.set();
			}

			bool remove() {
				if(!valuePresent)
					return false;
				ValueDestructor(*reinterpret_cast<ValueT*>(value.buffer));
				valuePresent = false;
				--map.cursize;
				PublicNode* collapse = this;
				while(collapse->children.empty() && !collapse->valuePresent && collapse->parent) {
					PublicNode* p = collapse->parent;
					p->children.erase(collapse->key);
					delete collapse;
					collapse = p;
				}
				return true;
			}

			void getKeyPath(util::Appender<KeyT>& sink) const {
				getKeyPathImpl(sink);
				sink.doneAppending();
			}

			PublicNode* clone(SequenceMap& newMap, PublicNode* newParent) const {
				util::Delete<PublicNode> newNode(new PublicNode(newMap, newParent, key));
				if(valuePresent)
					newNode->setValue(*reinterpret_cast<const ValueT*>(value.buffer));
				ConstChildIterator begin(children.begin()), end(children.end());
				for(; begin != end; ++begin) {
					util::Delete<PublicNode> newChild(begin->second->clone(newMap, *newNode));
					newNode->children[begin->first] = *newChild;
					newChild.set();
				}
				return newNode.set();
			}

			void clear() {
				ConstChildIterator begin(children.begin()), end(children.end());
				for(; begin != end; ++begin)
					delete begin->second;
				children.clear();
			}

			static void* operator new(size_t size) {
				return Allocator(static_cast<util::MemorySize>(size));
			}

			static void operator delete(void* address) {
				Deallocator(address);
			}

		};

		typedef PublicNode Node;

	  private:
		Node* root;

	  public:
		SequenceMap(const KeyT& rootKey = KeyT())
				: cursize(static_cast<util::MemorySize>(0u)), root(new Node(*this, NULL, rootKey)) {}

		SequenceMap(const SequenceMap& map)
				: cursize(static_cast<util::MemorySize>(0u)), root(map.root->clone(*this, NULL)) {}

		inline util::MemorySize size() const {
			return cursize;
		}

		inline PublicNode& getRoot() {
			return *root;
		}

		inline const PublicNode& getRoot() const {
			return *root;
		}

		void clear() {
			root->clear();
			root->remove();
		}

		typename PublicNode::template MaybeNode<false> get(const KeyT& key) {
			return root->get(key);
		}

		typename PublicNode::template MaybeNode<true> get(const KeyT& key) const {
			return const_cast<const Node*>(root)->get(key);
		}

		PublicNode& put(const KeyT& key) {
			return root->put(key);
		}

		PublicNode& put(const KeyT& key, const ValueT& value) {
			return root->put(key, value);
		}

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_SEQUENCEMAP_HPP */
