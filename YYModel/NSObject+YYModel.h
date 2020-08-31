//
//  NSObject+YYModel.h
//  YYModel <https://github.com/ibireme/YYModel>
//
//  Created by ibireme on 15/5/10.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 提供了一些数据模型的方法:
 
 * json转任意模型, 任意模型转json.
 * 通过 键-值对(NSDictionary) 的方式设置模型属性 (类似KVC).
 * `NSCoding`, `NSCopying`, `-hash` and `-isEqual:` 方法的重写与实现。
 
 参见' YYModel '协议，了解自定义方法。
 
 
 示例代码:
    
     ********************** json 转 模型 *********************
     @interface YYAuthor : NSObject
     @property (nonatomic, strong) NSString *name;
     @property (nonatomic, assign) NSDate *birthday;
     @end
     @implementation YYAuthor
     @end
 
     @interface YYBook : NSObject
     @property (nonatomic, copy) NSString *name;
     @property (nonatomic, assign) NSUInteger pages;
     @property (nonatomic, strong) YYAuthor *author;
     @end
     @implementation YYBook
     @end
    
     int main() {
         // json转模型
         YYBook *book = [YYBook yy_modelWithJSON:@"{\"name\": \"Harry Potter\", \"pages\": 256, \"author\": {\"name\": \"J.K.Rowling\", \"birthday\": \"1965-07-31\" }}"];
 
         // 模型转json
         NSString *json = [book yy_modelToJSONString];
         // {"author":{"name":"J.K.Rowling","birthday":"1965-07-31T00:00:00+0000"},"name":"Harry Potter","pages":256}
     }
 
     ********************** Coding/Copying/hash/equal *********************
     @interface YYShadow :NSObject <NSCoding, NSCopying>
     @property (nonatomic, copy) NSString *name;
     @property (nonatomic, assign) CGSize size;
     @end
 
     @implementation YYShadow
     - (void)encodeWithCoder:(NSCoder *)aCoder { [self yy_modelEncodeWithCoder:aCoder]; }
     - (id)initWithCoder:(NSCoder *)aDecoder { self = [super init]; return [self yy_modelInitWithCoder:aDecoder]; }
     - (id)copyWithZone:(NSZone *)zone { return [self yy_modelCopy]; }
     - (NSUInteger)hash { return [self yy_modelHash]; }
     - (BOOL)isEqual:(id)object { return [self yy_modelIsEqual:object]; }
     @end
 
 */
@interface NSObject (YYModel)

/**
 通过一个 json，创建和返回一个新的实例
 此方法是线程安全的。
 
 @参数： json  一个json对象，可能的类型有：`NSDictionary`, `NSString` or `NSData`。
 
 @返回： 一个通过json创建的新实例。 出错的情况下返回nil。
 */
+ (nullable instancetype)yy_modelWithJSON:(id)json;

/**
 通过一组 键-值对(NSDictionary)，创建和返回一个新的实例
 此方法是线程安全的。

 @参数： dictionary  一组能够映射实例属性的 键-值对(dictionary)
 无效的键值对将会被忽略。
 
 @返回： 一个通过 键-值对(dictionary) 创建的新实例，出错的情况下返回nil。

 @说明： 字典中的 key 和 value 将分别映射在模型的属性名，和属性值上。
 如果值得类型不发与属性相匹配，此方法将尝试根据如下规则，进行转化：
 
     `NSString` or `NSNumber` -> c number, such as BOOL, int, long, float, NSUInteger...
     `NSString` -> NSDate, parsed with format "yyyy-MM-dd'T'HH:mm:ssZ", "yyyy-MM-dd HH:mm:ss" or "yyyy-MM-dd".
     `NSString` -> NSURL.
     `NSValue` -> struct or union, such as CGRect, CGSize, ...
     `NSString` -> SEL, Class.
 */
+ (nullable instancetype)yy_modelWithDictionary:(NSDictionary *)dictionary;

/**
 通过json对象设置模型属性
 
 @说明： json中任何无效的数据都会别忽略.
 
 @参数： json  如下类型的json对象： `NSDictionary`, `NSString` or `NSData`，一一映射值至模型的属性上
 
 @返回： 是否成功。
 */
- (BOOL)yy_modelSetWithJSON:(id)json;

/**
 通过 键-值对(NSDictionary) 设置模型属性

 @参数： dic  能够映射至模型属性的键值对。
 字典中任何无效的键值对都会被忽略。
 
 @说明： 字典中的 key 和 value 将分别映射在模型的属性名，和属性值上。
 如果值得类型不发与属性相匹配，此方法将尝试根据如下规则，进行转化：

     `NSString`, `NSNumber` -> c number, such as BOOL, int, long, float, NSUInteger...
     `NSString` -> NSDate, parsed with format "yyyy-MM-dd'T'HH:mm:ssZ", "yyyy-MM-dd HH:mm:ss" or "yyyy-MM-dd".
     `NSString` -> NSURL.
     `NSValue` -> struct or union, such as CGRect, CGSize, ...
     `NSString` -> SEL, Class.
 
 @返回： 是否成功。
 */
- (BOOL)yy_modelSetWithDictionary:(NSDictionary *)dic;

/**
 模型转json对象
 
 @返回： 如下类型的json对象： `NSDictionary` or `NSArray`，出错的情况下返回nil。
 参考 [NSJSONSerialization isValidJSONObject] 获取更多信息。
 
 @说明： 任何无效的属性都会被忽略。
 如果待转化的模型是 数组、字典或集合，那么它们的内部模型也将被转换成json对象
 */
- (nullable id)yy_modelToJSONObject;

/**
 模型转json字符串的NSData类型

 @返回： json字符串的NSData类型, 出错的情况下返回nil。
 
 @说明： 任何无效的属性都会被忽略。
 如果待转化的模型是 数组、字典或集合，那么它们的内部模型也将被转换成json字符串的NSData类型数据
 */
- (nullable NSData *)yy_modelToJSONData;

/**
 模型转json字符串

 @返回： json字符串, 出错的情况下返回nil。
 
 @说明： 任何无效的属性都会被忽略。
 如果待转化的模型是 数组、字典或集合，那么它们的内部模型也将被转换成json字符串
 */
- (nullable NSString *)yy_modelToJSONString;

/**
 复制带有模型属性的实例。
 
 @返回： 一个复制的实例, 出错的情况下返回nil。
 */
- (nullable id)yy_modelCopy;

/**
 对模型的属性进行编码操作。
 
 @参数： aCoder  一个归档对象。
 */
- (void)yy_modelEncodeWithCoder:(NSCoder *)aCoder;

/**
 对模型的属性进行解码操作。
 
 @参数： aDecoder  一个归档对象。
 
 @返回： self 实例本身
 */
- (id)yy_modelInitWithCoder:(NSCoder *)aDecoder;

/**
 通过模型属性获取哈希码
 
 @返回： Hash code 哈希码
 */
- (NSUInteger)yy_modelHash;

/**
 基于属性的，与另一个模型的比较是否对等
 
 @参数： model  另一个对象
 
 @返回： `YES` 如果两个对象对等； 否则返回 `NO`。
 */
- (BOOL)yy_modelIsEqual:(id)model;

/**
 基于属性用于调试的描述方法。
 
 @返回： 描述模型内容的字符串。
 */
- (NSString *)yy_modelDescription;

@end



/**
 针对NSArray数组 提供了一些数据模型的方法。
 */
@interface NSArray (YYModel)

/**
 通过json-array数组，创建和返回模型数组
 此方法是线程安全的。

 @参数： cls  数组中实例的类型。
 @参数： json  如下类型的json数组： `NSArray`, `NSString` or `NSData`.
              示例: [{"name","Mary"},{name:"Joe"}]
 
 @返回： 模型数组, 出错的情况下返回nil。
 */
+ (nullable NSArray *)yy_modelArrayWithClass:(Class)cls json:(id)json;

@end



/**
 针对NSDictionary字典 提供了一些数据模型的方法。
 */
@interface NSDictionary (YYModel)

/**
 通过json，创建和返回模型字典
 此方法是线程安全的。

 @参数： cls  字典中实例的类型。
 @参数： json  如下类型的json字典： `NSDictionary`, `NSString` or `NSData`.
              示例: {"user1":{"name","Mary"}, "user2": {name:"Joe"}}
 
 @返回： 模型字典, 出错的情况下返回nil。
 */
+ (nullable NSDictionary *)yy_modelDictionaryWithClass:(Class)cls json:(id)json;
@end



/**
 如果默认的模型转换不能适应你的模型类，可以通过实现这个协议中的一个或多个方法，来改变默认 键-值 转换的过程。
 你 不需要 在类标题中引入和添加'<YYModel>'。
 */
@protocol YYModel <NSObject>
@optional

/**
 自定义属性映射器
 
 @说明：  如果 JSON/Dictionary 中的键(key) 无法匹配模型中的属性，
 实现该方法，并返回一个额外的映射表。
 
 示例:
    
    json:
        {
            "n":"Harry Pottery",
            "p": 256,
            "ext" : {
                "desc" : "A book written by J.K.Rowling."
            },
            "ID" : 100010
        }
 
    model:
        @interface YYBook : NSObject
        @property NSString *name;
        @property NSInteger page;
        @property NSString *desc;
        @property NSString *bookID;
        @end
        
        @implementation YYBook
        + (NSDictionary *)modelCustomPropertyMapper {
            return @{@"name"  : @"n",
                     @"page"  : @"p",
                     @"desc"  : @"ext.desc",
                     @"bookID": @[@"id", @"ID", @"book_id"]};
        }
        @end
 
 @返回： 属性映射表
 */
+ (nullable NSDictionary<NSString *, id> *)modelCustomPropertyMapper;

/**
 容器类(字典、数组、集合)中所需要存放数据类型的映射表
 
 @说明： 如果有个容器类, 例如 NSArray/NSSet/NSDictionary,
 实现此方法，并返回一个 属性->类 的映射表，告知什么类型的对象将被添加至 数组/集合/字典。
 
  示例:
        @class YYShadow, YYBorder, YYAttachment;
 
        @interface YYAttributes
        @property NSString *name;
        @property NSArray *shadows;
        @property NSSet *borders;
        @property NSDictionary *attachments;
        @end
 
        @implementation YYAttributes
        + (NSDictionary *)modelContainerPropertyGenericClass {
            return @{@"shadows" : [YYShadow class],
                     @"borders" : YYBorder.class,
                     @"attachments" : @"YYAttachment" };
        }
        @end
 
 @返回： 类的映射表
 */
+ (nullable NSDictionary<NSString *, id> *)modelContainerPropertyGenericClass;

/**
 如果你在 json转模型的过程中，需要根据实际情况创建不同类型的实例，可以使用此方法来基于字典的数据选择自定义的类型。
 
 @说明： 如果有模型实现了该方法, 它会在你使用  `+modelWithJSON:`, `+modelWithDictionary:`的期间被回调，以确定该生成什么类，并转换父对象的属性。

 (both singular and containers via `+modelContainerPropertyGenericClass`).
 
 示例:
        @class YYCircle, YYRectangle, YYLine;
 
        @implementation YYShape

        + (Class)modelCustomClassForDictionary:(NSDictionary*)dictionary {
            if (dictionary[@"radius"] != nil) {
                return [YYCircle class];
            } else if (dictionary[@"width"] != nil) {
                return [YYRectangle class];
            } else if (dictionary[@"y2"] != nil) {
                return [YYLine class];
            } else {
                return [self class];
            }
        }

        @end

 @参数： dictionary  json/key-value 字典
 
 @返回： 通过具体情况返回对应类的类型，返回 `nil` 则说明使用当前类

 */
+ (nullable Class)modelCustomClassForDictionary:(NSDictionary *)dictionary;

/**
 在模型的转换过程中，黑名单里的属性都会被忽略。
 返回nil可以忽略这个功能
 
 @返回： 属性名的数组。
 */
+ (nullable NSArray<NSString *> *)modelPropertyBlacklist;

/**
 在模型的转换过程中，不在白名单里的属性都会被忽略。
 返回nil可以忽略这个功能

 @返回： 属性名的数组。
 */
+ (nullable NSArray<NSString *> *)modelPropertyWhitelist;

/**
 此方法的行为与`- (BOOL)modelCustomTransformFromDictionary:(NSDictionary *)dic;` 类似，
 但是在模型转换之前被调用。
 
 @说明： 如果模型实现这个方法, 它会在
 `+modelWithJSON:`, `+modelWithDictionary:`, `-modelSetWithJSON:` and `-modelSetWithDictionary:`这些方法实现前调用。
 如果此方法返回nil, 那么转换过程中会忽略这个模型。
 
 @参数： dic  json/键值 字典。
 
 @返回： 返回修改后的字典，或nil 来忽略这个模型。
 */
- (NSDictionary *)modelCustomWillTransformFromDictionary:(NSDictionary *)dic;

/**
 如果默认的模型转换不能适应你的模型类，可以通过实现此方法来添加额外的转换。
 实现此方法以执行额外的处理。你还可以使用此方法验证模型属性的有效性。
 
 @说明： 如果你实现了此方法, 它会在 `+modelWithJSON:`, `+modelWithDictionary:`, `-modelSetWithJSON:` and `-modelSetWithDictionary:`完成后被调用。
 如果此方法返回 NO, 转换过程中会忽略这个模型。
 
 @参数： dic  json/键值 字典。
 
 @返回： 返回YES说明模型是有效的, 或者返回 NO 忽略这个模型
 */
- (BOOL)modelCustomTransformFromDictionary:(NSDictionary *)dic;

/**
 如果默认的模型转换不能适应你的模型类，可以通过实现此方法来添加额外的转换。
 实现此方法以执行额外的处理。你还可以使用此方法验证json字典的有效性。

 @说明： 如果你实现了此方法, 它会在 `-modelToJSONObject` and `-modelToJSONString` 完成后被回调。
 如果此方法返回 NO, 转换过程中会忽略这个json字典。

 @参数： dic  json 字典.
 
 @返回： 返回YES说明模型是有效的, 或者返回 NO 忽略这个模型
 */
- (BOOL)modelCustomTransformToDictionary:(NSMutableDictionary *)dic;

@end

NS_ASSUME_NONNULL_END
